#include "ComponentTransform.h"
#include "External Libraries/ImGui/imgui.h"
#include "External Libraries/MathGeoLib/include/Math/float3x3.h"
#include "GameObject.h"

#define M_PI 3.14159265358979323846f

ComponentTransform::ComponentTransform(GameObject* owner, float3 position, Quat rotation, float3 size) :Component(Component_Type::Transform, owner, "Transform"), position(position), rotation(rotation), size(size)
{
	euler = QuaternionToEuler(rotation);
	SetMatrices();
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Update()
{
	if (!needed_to_update)
		return;
	rotation = EulerToQuaternion(euler);
	SetMatrices();
}

float3 ComponentTransform::GetPosition()const
{
	return position;
}

Quat ComponentTransform::GetRotation()const
{
	return rotation;
}

float3 ComponentTransform::GetRotation(bool convert)
{
	return QuaternionToEuler(rotation);
}

float3 ComponentTransform::GetSize()const
{
	return size;
}

float3 ComponentTransform::QuaternionToEuler(Quat q)
{
	float3 rotation_euler = q.ToEulerXYZ();
	//rotation_euler *= RADTODEG;
	return rotation_euler;
}

Quat ComponentTransform::EulerToQuaternion(float3 eu)
{
	//eu *= DEGTORAD;
	Quat q = Quat::FromEulerXYZ(eu.x, eu.y, eu.z);	
	return q;
}

float4x4 ComponentTransform::GetGlobalMatrix() const
{
	return global_matrix;
}

float4x4 ComponentTransform::GetGlobalMatrixTransposed() const
{
	return global_matrix_transposed;
}

bool ComponentTransform::SaveComponent(JsonObj& obj)
{
	JsonArray arr = obj.AddArray(this->name.c_str());
	JsonObj _obj;
	_obj.AddFloat4x4("GlobalMatrix",GetGlobalMatrix());
	arr.AddObject(_obj);
	return true;
}

void ComponentTransform::SetTransformation(float3 pos, Quat rot, float3 size)
{
	position = pos;
	rotation = rot;
	euler = QuaternionToEuler(rotation);
	this->size = size;
	SetMatrices();

}

void ComponentTransform::SetPosition(float3 pos)
{
	position = pos;
	SetMatrices();
}

void ComponentTransform::SetRotation(Quat rot)
{
	rotation = rot;
	euler = QuaternionToEuler(rotation);
	SetMatrices();
}

void ComponentTransform::SetMatrices()
{
	local_matrix = local_matrix.FromTRS(position, rotation, size);
	if (owner->parent != nullptr) {
		ComponentTransform* parent_trans = (ComponentTransform*)owner->parent->GetComponent(Component_Type::Transform);
		global_matrix = parent_trans->GetGlobalMatrix() * local_matrix;
	}
	else global_matrix = local_matrix;
	if (!global_matrix.Equals(global_matrix2)&&needed_to_update_only_children)
		LOG("HEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE");
	global_matrix_transposed = global_matrix.Transposed();
}

void ComponentTransform::SetMatricesWithNewParent(float4x4 parent_global_matrix)
{
	local_matrix = parent_global_matrix.Inverted() * global_matrix;
	local_matrix.Decompose(position, rotation, size);
	global_matrix2 = parent_global_matrix * local_matrix;
	if (!global_matrix.Equals(global_matrix2))
		LOG("HEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE");
	euler = QuaternionToEuler(rotation);
	needed_to_update=true;
	needed_to_update_only_children = true;
}

void ComponentTransform::Inspector()
{
	float null = 0;
	ImGui::PushID(name.c_str());
	Component::Inspector();

	ImGui::Separator();

	ImGui::Columns(4, "", false);

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Position");

	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("X");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::DragFloat("##0", (active && owner->active) ? &position.x : &null, 0.01f) && (active && owner->active))
		needed_to_update = true;
	ImGui::PopItemWidth();

	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Y");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::DragFloat("##1", (active && owner->active) ? &position.y : &null, 0.01f) && (active && owner->active))
		needed_to_update = true;
	ImGui::PopItemWidth();

	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Z");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::DragFloat("##2", (active && owner->active) ? &position.z : &null, 0.01f) && (active && owner->active))
		needed_to_update = true;
	ImGui::PopItemWidth();

	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Rotation");

	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("X");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::DragFloat("##3", (active && owner->active) ? &euler.x : &null, 0.01f) && (active && owner->active))
		needed_to_update = true;
	ImGui::PopItemWidth();

	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Y");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::DragFloat("##4", (active && owner->active) ? &euler.y : &null, 0.01f) && (active && owner->active))
		needed_to_update = true;
	ImGui::PopItemWidth();

	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Z");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::DragFloat("##5", (active && owner->active) ? &euler.z : &null, 0.01f) && (active && owner->active))
		needed_to_update = true;
	ImGui::PopItemWidth();

	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Size");

	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("X");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::DragFloat("##6", (active && owner->active) ? &size.x : &null, 0.01f) && (active && owner->active))
		needed_to_update = true;
	ImGui::PopItemWidth();

	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Y");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::DragFloat("##7", (active && owner->active) ? &size.y : &null, 0.01f) && (active && owner->active))
		needed_to_update = true;
	ImGui::PopItemWidth();

	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Z");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::DragFloat("##8", (active && owner->active) ? &size.z : &null, 0.01f) && (active && owner->active))
		needed_to_update = true;
	ImGui::PopItemWidth();

	ImGui::Columns(1, "", false);
	ImGui::Separator();
	ImGui::PopID();
}

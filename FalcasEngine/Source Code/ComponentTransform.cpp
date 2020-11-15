#include "ComponentTransform.h"
#include "External Libraries/ImGui/imgui.h"
#include "GameObject.h"

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
	needed_to_update = false;
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
	float3 rotation_euler;
	if (rotation.x * rotation.y + rotation.z * rotation.w == 0.5) {
		rotation_euler.x = 0;
		rotation_euler.z = 2 * atan2(rotation.x, rotation.w);
	}
	else if (rotation.x * rotation.y + rotation.z * rotation.w == -0.5) {
		rotation_euler.x = 0;
		rotation_euler.z = -2 * atan2(rotation.x, rotation.w);
	}
	else {
		rotation_euler.x = atan2(2 * rotation.x * rotation.w - 2 * rotation.y * rotation.z, 1 - 2 * pow(rotation.x, 2) - 2 * pow(rotation.z, 2));
		rotation_euler.z = atan2(2 * rotation.y * rotation.w - 2 * rotation.x * rotation.z, 1 - 2 * pow(rotation.y, 2) - 2 * pow(rotation.z, 2));
	}
	rotation_euler.y = asin(2 * rotation.x * rotation.y + 2 * rotation.z * rotation.w);

	return rotation_euler;
}

Quat ComponentTransform::EulerToQuaternion(float3 eu)
{
	Quat q;
	q.w = cos(eu.z / 2) * cos(eu.y / 2) * cos(eu.x / 2) - sin(eu.z / 2) * sin(eu.y / 2) * sin(eu.x / 2);
	q.x = sin(eu.z / 2) * sin(eu.y / 2) * cos(eu.x / 2) + cos(eu.z / 2) * cos(eu.y / 2) * sin(eu.x / 2);
	q.y = sin(eu.z / 2) * cos(eu.y / 2) * cos(eu.x / 2) + cos(eu.z / 2) * sin(eu.y / 2) * sin(eu.x / 2);
	q.z = cos(eu.z / 2) * sin(eu.y / 2) * cos(eu.x / 2) - sin(eu.z / 2) * cos(eu.y / 2) * sin(eu.x / 2);
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
	global_matrix_transposed = global_matrix.Transposed();
}

void ComponentTransform::Inspector()
{
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
	if(ImGui::DragFloat("##0", &position.x, 0.01f))
		needed_to_update = true;
	ImGui::PopItemWidth();
	
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Y");
	
	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::DragFloat("##1", &position.y, 0.01f))
		needed_to_update = true;
	ImGui::PopItemWidth();
	
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Z");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if(ImGui::DragFloat("##2", &position.z, 0.01f))
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
	if(ImGui::DragFloat("##3", &euler.x, 0.01f))
		needed_to_update = true;
	ImGui::PopItemWidth();
	
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Y");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if(ImGui::DragFloat("##4", &euler.y, 0.01f))
		needed_to_update = true;
	ImGui::PopItemWidth();
	
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Z");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if(ImGui::DragFloat("##5", &euler.z, 0.01f))
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
	if(ImGui::DragFloat("##6", &size.x, 0.01f))
		needed_to_update = true;
	ImGui::PopItemWidth();
	
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Y");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if(ImGui::DragFloat("##7", &size.y, 0.01f))
		needed_to_update = true;
	ImGui::PopItemWidth();
	
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Z");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if(ImGui::DragFloat("##8", &size.z, 0.01f))
		needed_to_update = true;
	ImGui::PopItemWidth();
	
	ImGui::Columns(1, "", false);
	ImGui::Separator();
	ImGui::PopID();
}

#include "ComponentTransform2D.h"
#include "ComponentTransform.h"
#include "External Libraries/ImGui/imgui.h"
#include "External Libraries/MathGeoLib/include/Math/float3x3.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "ComponentCamera.h"

#define M_PI 3.14159265358979323846f

ComponentTransform2D::ComponentTransform2D(GameObject* owner, float2 position, Quat rotation, float2 size) :Component(Component_Type::Transform2D, owner, "Transform2D"), position(position), quat_rotation(rotation),
size(size), z_depth(20), rotation(QuaternionToEuler(quat_rotation))
{
	SetMatrices();
	float* camera_view_matrix = App->renderer3D->camera->GetViewMatrix();
}

ComponentTransform2D::~ComponentTransform2D()
{
}

void ComponentTransform2D::Update()
{
	
	SetMatrices();
}

float2 ComponentTransform2D::GetPosition()const
{
	return position;
}

float3 ComponentTransform2D::GetRotation()const
{
	return rotation;
}

Quat ComponentTransform2D::GetRotationQuaternion()const
{
	return quat_rotation;
}

float2 ComponentTransform2D::GetSize()const
{
	return size;
}

float4x4 ComponentTransform2D::GetGlobalMatrix() const
{
	return global_matrix;
}

float4x4 ComponentTransform2D::GetGlobalMatrixTransposed() const
{
	return global_matrix_transposed;
}

bool ComponentTransform2D::SaveComponent(JsonObj& obj)
{
	obj.AddFloat4x4("GlobalMatrix", GetGlobalMatrix());
	return true;
}

void ComponentTransform2D::SetTransformation(float2 pos, Quat rot, float2 size)
{
	position = pos;
	quat_rotation = rot;
	rotation = QuaternionToEuler(rot);
	this->size = size;
	SetMatrices();
	needed_to_update = true;

}

void ComponentTransform2D::SetPosition(float2 pos)
{
	position = pos;
	SetMatrices();
}


void ComponentTransform2D::SetRotation(Quat rot)
{
	quat_rotation = rot;
	rotation = QuaternionToEuler(rot);
	SetMatrices();
}

void ComponentTransform2D::SetRotation(float3 rot)
{
	rotation = rot;
	quat_rotation = EulerToQuaternion(rot);
	SetMatrices();
}

void ComponentTransform2D::SetSize(float2 size)
{
	this->size = size;
	SetMatrices();
}
void ComponentTransform2D::SetMatrices()
{
	float3 movement= { position.x, position.y, z_depth };
	Quat rot= ((ComponentTransform*)App->renderer3D->camera->owner->GetComponent(Component_Type::Transform))->GetRotation();
	//rot = rot.RotateX(rotation.x);
	movement = rot * movement;
	float3 pos, s;
	pos = ((ComponentTransform*)App->renderer3D->camera->owner->GetComponent(Component_Type::Transform))->GetPosition();
	pos += movement;
	Quat rot2 = Quat::identity;
	Quat rotx = rot2.RotateX(rotation.x);
	Quat roty = rot2.RotateY(rotation.y);
	Quat rotz = rot2.RotateZ(rotation.z);
	rot = rot*rot2 * rotx * roty * rotz;
	
	s = { size.x,size.y,1 };
	
	local_matrix = local_matrix.FromTRS(pos, rot, s);
	if (owner->parent != nullptr) {
		if (owner->parent->IsUI()) {
			ComponentTransform2D* parent_trans = (ComponentTransform2D*)owner->parent->GetComponent(Component_Type::Transform2D);
			global_matrix = parent_trans->GetGlobalMatrix() * local_matrix;
		}
		else {
			ComponentTransform* parent_trans = (ComponentTransform*)owner->parent->GetComponent(Component_Type::Transform);
			global_matrix = parent_trans->GetGlobalMatrix() * local_matrix;
		}
	}
	else global_matrix = local_matrix;
	global_matrix_transposed = global_matrix.Transposed();
	
}

void ComponentTransform2D::SetMatricesWithNewParent(float4x4 parent_global_matrix)
{
	local_matrix = parent_global_matrix.Inverted() * global_matrix;
	float3 pos, s;
	Quat rot;
	local_matrix.Decompose(pos, rot, s);
	position = { pos.x,pos.y };
	float3 rotate = QuaternionToEuler(rot);
	rotation = rotate;
	size = { s.x,s.y };
	needed_to_update = true;
	needed_to_update_only_children = true;
}

float3 ComponentTransform2D::QuaternionToEuler(Quat q)
{
	float3 rotation_euler = q.ToEulerXYZ();
	rotation_euler *= RADTODEG;
	return rotation_euler;
}

Quat ComponentTransform2D::EulerToQuaternion(float3 eu)
{
	eu *= DEGTORAD;
	Quat q = Quat::FromEulerXYZ(eu.x, eu.y, eu.z);
	return q;
}

Quat ComponentTransform2D::LookAt(const float3& point)
{

	float3 vector = {position.x, position.x, 0};
	vector = point - vector;

	float3x3 matrix = float3x3::LookAt(float3::unitZ, vector.Normalized(), float3::unitY, float3::unitY);
	return matrix.ToQuat();
	
}

void ComponentTransform2D::Inspector()
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
	if (ImGui::DragFloat("##2", (active && owner->active) ? &z_depth : &null, 0.01f) && (active && owner->active))
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
	if (ImGui::DragFloat("##3", (active && owner->active) ? &rotation.x : &null, 0.01f) && (active && owner->active))
		needed_to_update = true;
	ImGui::PopItemWidth();

	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Y");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::DragFloat("##4", (active && owner->active) ? &rotation.y : &null, 0.01f) && (active && owner->active))
		needed_to_update = true;
	ImGui::PopItemWidth();

	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Z");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::DragFloat("##5", (active && owner->active) ? &rotation.z : &null, 0.01f) && (active && owner->active))
		needed_to_update = true;
	ImGui::PopItemWidth();

	ImGui::Columns(3, "", false);

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

	ImGui::Columns(1, "", false);
	ImGui::Separator();
	ImGui::PopID();
}

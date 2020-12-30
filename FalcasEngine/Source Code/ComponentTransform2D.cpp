#include "ComponentTransform2D.h"
#include "ComponentTransform.h"
#include "External Libraries/ImGui/imgui.h"
#include "External Libraries/MathGeoLib/include/Math/float3x3.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "ComponentCamera.h"

#define M_PI 3.14159265358979323846f

ComponentTransform2D::ComponentTransform2D(GameObject* owner, float2 position, Quat rotation, float2 size) :Component(Component_Type::Transform2D, owner, "Transform2D"), position(position),
size(size), z_depth(20)
{
	this->rotation = QuaternionToEuler(rotation);
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

void ComponentTransform2D::SetTransformation(float3 pos, Quat rot, float2 size, bool guizmo_size)
{

	float3 new_pos, dummy;
	Quat new_rot;
	global_matrix.Decompose(new_pos, new_rot, dummy);
	
	new_pos -= pos;
	position.x += new_pos.x;
	position.y -= new_pos.y;
	z_depth += new_pos.z;

	if (!guizmo_size) {
		float rot_z = QuaternionToEuler(rot).z;
		float new_rot_z = QuaternionToEuler(new_rot).z;
		rotation.z += rot_z - new_rot_z;
	}
	
	this->size = size;
	needed_to_update = true;

}

void ComponentTransform2D::SetPosition(float2 pos)
{
	position = pos;
	SetMatrices();
}


void ComponentTransform2D::SetRotation(Quat rot)
{
	rotation = QuaternionToEuler(rot);
	SetMatrices();
}

void ComponentTransform2D::SetRotation(float3 rot)
{
	rotation = rot;
	SetMatrices();
}

void ComponentTransform2D::SetSize(float2 size)
{
	this->size = size;
	SetMatrices();
}

void ComponentTransform2D::UpdateMatrixBillboard()
{
	ComponentTransform* trans = (ComponentTransform*)App->renderer3D->camera->owner->GetComponent(Component_Type::Transform);
	Quat rot = trans->GetRotation();
	float3 pos = trans->GetPosition();
	matrix_billboard = matrix_billboard.FromTRS(pos, rot, { 1,1,1 });
}

void ComponentTransform2D::SetMatrices()
{
	UpdateMatrixBillboard();

	float3 pivot_world = { pivot_position.x + position.x, pivot_position.y + position.y, 0 };
	float3 rotation_in_gradians = rotation *DEGTORAD;
	Quat rotate = Quat::identity * Quat::identity.RotateZ(rotation_in_gradians.z);
	
	matrix_pivot = matrix_pivot.FromTRS(pivot_world, rotate, { 1,1,1 });


	local_matrix = local_matrix.FromTRS({ -pivot_position.x,-pivot_position.y,z_depth }, Quat::identity, { size.x,size.y,1 } );

	if (owner->parent != nullptr) {
		if (owner->parent->IsUI()) {
			ComponentTransform2D* parent_trans = (ComponentTransform2D*)owner->parent->GetComponent(Component_Type::Transform2D);
			matrix_parent = parent_trans->GetGlobalMatrix();
		}
		else {
			ComponentTransform* parent_trans = (ComponentTransform*)owner->parent->GetComponent(Component_Type::Transform);
			matrix_parent = parent_trans->GetGlobalMatrix();
		}
	}
	else matrix_parent = float4x4::identity;
	global_matrix = matrix_parent * matrix_billboard * matrix_pivot * local_matrix;
	global_matrix_transposed = global_matrix.Transposed();
	needed_to_update = false;
}

void ComponentTransform2D::SetMatricesWithNewParent(float4x4 parent_global_matrix)
{
	float4x4 local_matrix_with_rotation = matrix_billboard.Inverted() * parent_global_matrix.Inverted() * global_matrix;
	float3 pos, s;
	Quat rot;
	local_matrix_with_rotation.Decompose(pos, rot, s);
	rotation = QuaternionToEuler(rot);

	local_matrix = matrix_pivot.Inverted() * local_matrix_with_rotation;
	
	local_matrix.Decompose(pos, rot, s);

	position = { pos.x,pos.y };
	z_depth = pos.z;
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

float2 ComponentTransform2D::CalculateMovement(float4x4 matrix, float2 goal)
{
	float2 movement;
	movement.y = (matrix[0][0] * (goal.y - matrix[1][3]) - matrix[1][0] * (goal.x - matrix[0][3])) / (matrix[1][1] * matrix[0][0] - matrix[0][1] * matrix[1][0]);
	movement.x = (goal.x - matrix[0][1] * movement.y - matrix[0][3]) / matrix[0][0];
	float second_option= (goal.y - matrix[1][1] * movement.y - matrix[1][3]) / matrix[1][0];
	if (movement.x != goal.x && ((second_option - goal.x<0 && second_option - goal.x> movement.x) || (second_option - goal.x > 0 && second_option - goal.x < movement.x))) {
		movement.x = second_option;
	}
	
	return movement;
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

	ImGui::Columns(1, "", false);
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Rotation");

	

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

	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Pivot pos");

	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("X");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::DragFloat("##8", (active && owner->active) ? &pivot_position.x : &null, 0.01f) && (active && owner->active))
		needed_to_update = true;
	ImGui::PopItemWidth();

	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Y");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::DragFloat("##9", (active && owner->active) ? &pivot_position.y : &null, 0.01f) && (active && owner->active)) 
		needed_to_update = true;
	ImGui::PopItemWidth();

	ImGui::Columns(1, "", false);
	ImGui::Separator();
	ImGui::PopID();
}

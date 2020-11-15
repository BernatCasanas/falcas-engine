#include "ComponentCamera.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "External Libraries/ImGui/imgui.h"

//////TEMPORAL
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"

ComponentCamera::ComponentCamera(GameObject* owner, ComponentTransform* trans) :Component(Component_Type::Camera, owner, "Camera"), trans(trans)
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetPos(trans->GetPosition());
	frustum.SetFront({ 0,0,1 });
	frustum.SetUp({ 0,1,0 });

	UpdateFrustum();

}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::UpdateFrustum()
{
	frustum.SetViewPlaneDistances(near_plane_distance, far_plane_distance);
	field_of_view_horizontal = 2 * Atan(Tan(field_of_view_vertical / 2)*(width/height));
	frustum.SetPerspective(field_of_view_vertical, field_of_view_horizontal);
	update_projection_matrix = true;
}
void ComponentCamera::Update()
{
	if (camera_active)
		App->renderer3D->camera = this;
	else
		App->renderer3D->camera = App->camera->camera;
	frustum.SetPos(trans->GetPosition());
	frustum.SetFront(float3::unitZ * float3x3::FromQuat(trans->GetRotation()));
	frustum.SetUp(float3::unitY * float3x3::FromQuat(trans->GetRotation()));
	if (!App->scene_intro->GetDimensionsWindow(width, height) &&!needed_to_update)
		return;
	UpdateFrustum();
}

bool ComponentCamera::GetIfIsFrustumCulling() const
{
	if (active)
		return frustum_culling;
	else
		return false;
}

float* ComponentCamera::GetProjectionMatrix() const
{
	static float4x4 ProjectionMatrix;
	ProjectionMatrix= frustum.ProjectionMatrix().Transposed();
	return (float*)ProjectionMatrix.v;
}

float* ComponentCamera::GetViewMatrix() const
{
	float3 pos = trans->GetPosition();
	const float3x4 ViewMatrixFrustum = frustum.ViewMatrix();
	float4x4 ViewMatrix = ViewMatrixFrustum;
	ViewMatrix.SetRow(3, { -Dot(ViewMatrix.Row3(0), pos), -Dot(ViewMatrix.Row3(1), pos), -Dot(ViewMatrix.Row3(2), pos), 1.0f });
	ViewMatrix.Transpose();
	return (float*)ViewMatrix.v;
	
}

void ComponentCamera::Inspector()
{
	ImGui::PushID(name.c_str());
	Component::Inspector();

	ImGui::Separator();

	ImGui::Checkbox("View Camera", &camera_active);

	ImGui::Checkbox("Camera Culling", &frustum_culling);

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Near Plane Distance");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::DragFloat("##0", &near_plane_distance, 0.01f))
		needed_to_update = true;
	ImGui::PopItemWidth();

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Far Plane Distance");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::DragFloat("##1", &far_plane_distance, 0.01f))
		needed_to_update = true;
	ImGui::PopItemWidth();

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Field of View Vertical");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::DragFloat("##2", &field_of_view_vertical, 0.01f))
		needed_to_update = true;
	ImGui::PopItemWidth();

	ImGui::Separator();
	ImGui::PopID();
}


#include "ComponentCamera.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "External Libraries/ImGui/imgui.h"
#include "ModuleRenderer3D.h"
#include "ModuleCentralEditor.h"

ComponentCamera::ComponentCamera(GameObject* owner, ComponentTransform* trans) :Component(Component_Type::Camera, owner, "Camera"), trans(trans)
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetPos(trans->GetPosition());
	frustum.SetFront({ 0,0,1 });
	frustum.SetUp({ 0,1,0 });
	if (owner->id < 0)
		show_frustum = false;
	UpdateFrustum();

}

ComponentCamera::~ComponentCamera()
{
	if (camera_active&& owner->id >= 0) {
		App->renderer3D->ChangeCameraActive(nullptr);
	}
	if (frustum_culling && owner->id >= 0) {
		App->renderer3D->ChangeCullingCamera(nullptr);
	}
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
	frustum.SetPos(trans->GetPosition());
	frustum.SetFront(float3x3::FromQuat(trans->GetRotation()) * float3::unitZ);
	frustum.SetUp(float3x3::FromQuat(trans->GetRotation()) * float3::unitY);
	if (App->central_editor->frustums||show_frustum)
		App->renderer3D->camera_frustums.push_back(frustum);
	if (!App->scene_intro->GetDimensionsWindow(width, height) &&!needed_to_update)
		return;
	UpdateFrustum();
	needed_to_update = false;
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
	static float4x4 ViewMatrix;
	ViewMatrix = frustum.ViewMatrix();
	ViewMatrix.Transpose();
	return (float*)ViewMatrix.v;
	
}


void ComponentCamera::Inspector()
{
	bool falsed = false;
	float null = 0;

	ImGui::PushID(name.c_str());
	Component::Inspector();

	ImGui::Separator();

	ImGui::Checkbox("Show Camera Frustum", (active&&owner->active) ? &show_frustum : &falsed);
		
	if (ImGui::Checkbox("View Camera", (active && owner->active) ? &camera_active:&falsed) && (active && owner->active)) {
		if (camera_active) {
			App->renderer3D->ChangeCameraActive(this);
			show_frustum = false;
		}
		else {
			App->renderer3D->ChangeCameraActive(nullptr);
			show_frustum = true;
		}
	}
	

	if (ImGui::Checkbox("Camera Culling", (active && owner->active) ?&frustum_culling:&falsed) && (active && owner->active)) {
		if (frustum_culling)
			App->renderer3D->ChangeCullingCamera(this);
		else
			App->renderer3D->ChangeCullingCamera(nullptr);
	}

	if (active == false || owner->active == false) {
		if (frustum_culling) {
			App->renderer3D->ChangeCullingCamera(nullptr);
			//frustum_culling = false;
		}
		if (camera_active) {
			App->renderer3D->ChangeCameraActive(nullptr);
		}
	}

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Near Plane Distance");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::DragFloat("##0", (active && owner->active) ? &near_plane_distance:&null, 0.01f) && (active && owner->active))
		needed_to_update = true;
	ImGui::PopItemWidth();

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Far Plane Distance");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::DragFloat("##1", (active && owner->active) ? &far_plane_distance : &null, 0.01f) && (active && owner->active))
		needed_to_update = true;
	ImGui::PopItemWidth();

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Field of View Vertical");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::DragFloat("##2", (active && owner->active) ? &field_of_view_vertical : &null, 0.01f) && (active && owner->active))
		needed_to_update = true;
	ImGui::PopItemWidth();

	ImGui::Separator();
	ImGui::PopID();
}


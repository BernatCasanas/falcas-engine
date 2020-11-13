#include "ComponentCamera.h"
#include "External Libraries/ImGui/imgui.h"

ComponentCamera::ComponentCamera(GameObject* owner, float3 pos):Component(Component_Type::Camera,owner,"Camera")
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetPos({ 0,0,0 });
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
	frustum.SetPerspective(field_of_view_vertical, field_of_view_horizontal);

}
void ComponentCamera::Update()
{
	if (!needed_to_update)
		return;
	UpdateFrustum();
}

void ComponentCamera::Inspector()
{
	ImGui::PushID(name.c_str());
	Component::Inspector();

	ImGui::Separator();

	
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

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Field of View Horizontal");

	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::DragFloat("##3", &field_of_view_horizontal, 0.01f))
		needed_to_update = true;
	ImGui::PopItemWidth();

	ImGui::Separator();
	ImGui::PopID();
}


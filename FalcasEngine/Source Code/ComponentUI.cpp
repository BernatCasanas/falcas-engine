#include "ComponentUI.h"
#include "Application.h"
#include "ModuleResources.h"
#include "ResourceMesh.h"
#include "ModuleUI.h"
#include "GameObject.h"
#include "ComponentTransform2D.h"


#include "ModuleInput.h"
#include "ModuleSceneIntro.h"

ComponentUI::ComponentUI(Component_Type type, GameObject* owner, std::string name) : Component(type, owner, name)
{
	resource_mesh = (ResourceMesh*)App->resources->RequestResource(App->UI->mesh_plane_id);
	App->UI->UIs.push_back(owner);
	id_vector_uis = App->UI->UIs.size() - 1;
}

ComponentUI::~ComponentUI()
{
	if (resource_mesh != nullptr && !App->resources->isResourcesMapEmpty()) {
		App->resources->FreeResource(resource_mesh);
	}
	resource_mesh = nullptr;
	App->UI->DeleteUI(id_vector_uis);
}

void ComponentUI::Render()
{
}

void ComponentUI::OnTriggered(ComponentUI* component_ui)
{
	if (listener == nullptr)
		return;
	listener->OnTriggered(component_ui);
}


bool ComponentUI::CheckMouseHovering()
{
	if (!is_focusable)
		return false;

	UpdateTriangles();
	
	float scene_x, scene_y, scene_width, scene_height;
	App->scene_intro->GetSceneDimensions(scene_x, scene_y, scene_width, scene_height);
	float3 mouse = { 0,0,0 };
	mouse.x = App->input->GetMouseX() - scene_x;
	mouse.y = App->input->GetMouseY() - scene_y;
	if (triangle1.Contains(mouse) || triangle2.Contains(mouse)) {
		is_mouse_hover = true;
	}
	else {
		is_mouse_hover = false;
	}
	return is_mouse_hover;
}

void ComponentUI::UpdateTriangles()
{
	float3 min = resource_mesh->GetAABB().minPoint;
	float3 max = resource_mesh->GetAABB().maxPoint;
	float2 min_p = { min.x,min.y };
	float2 max_p = { max.x,max.y };
	float2 third_p = { min.x, max.y };
	float2 third_p_second_tri = { max.x,min.y };
	((ComponentTransform2D*)owner->GetComponent(Component_Type::Transform2D))->GetTrianglePoints(min_p, max_p, third_p, third_p_second_tri);
	min = { min_p.x,min_p.y,0 };
	max = { max_p.x,max_p.y,0 };
	float3 third = { third_p.x,third_p.y,0 };
	triangle1 = { min,max,third };
	third = { third_p_second_tri.x,third_p_second_tri.y,0 };
	triangle2 = { min,max,third };

}

void ComponentUI::IsClicked(bool clicked_with_mouse)
{
	is_clicked_with_enter = false;
	if (!is_focusable || (!is_mouse_hover && clicked_with_mouse) || (!clicked_with_mouse && !is_focused)) {
		is_focused = false;
		App->UI->focus_ui_id = -1;
		return;
	}
	if (!is_clicked) {
		is_clicked_first_frame = true;
		if (!clicked_with_mouse) {
			is_clicked_with_enter = true;
		}
	}
	else {
		is_clicked_first_frame = false;
	}
	is_clicked = true;
	is_focused = true;
	App->UI->focus_ui_id = id_vector_uis;
}

void ComponentUI::StoppedClicking(bool clicked_with_mouse)
{
	is_clicked = false;
	is_clicked_first_frame = false;
	if (clicked_with_mouse && type != Component_Type::Inputbox) {
		is_focused = false;
	}
}

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
}

ComponentUI::~ComponentUI()
{
	if (resource_mesh != nullptr && !App->resources->isResourcesMapEmpty()) {
		App->resources->FreeResource(resource_mesh);
	}
	resource_mesh = nullptr;
}

void ComponentUI::Render()
{
}

void ComponentUI::CheckMouseHovering()
{
	UpdateTriangles();
	
	float scene_x, scene_y, scene_width, scene_height;
	App->scene_intro->GetSceneDimensions(scene_x, scene_y, scene_width, scene_height);
	float3 mouse = { 0,0,0 };
	mouse.x = App->input->GetMouseX() - scene_x;
	mouse.y = App->input->GetMouseY() - scene_y;
	/*LOG("%f, %f", mouse_x, mouse_y);
	LOG("%f, %f", triangle1.a.x, triangle1.a.y);
	LOG("%f, %f", triangle1.b.x, triangle1.b.y);
	LOG("%f, %f", triangle1.c.x, triangle1.c.y);
	LOG("%f, %f", triangle2.c.x, triangle2.c.y);*/
	if (triangle1.Contains(mouse) || triangle2.Contains(mouse)) {
		is_mouse_hover = true;
	}
	else {
		is_mouse_hover = false;
	}
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

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

void ComponentUI::GetPlane()
{
	/*for (int i = 0; i < resource_mesh->num_vertices; i++) {
		LOG("%d, %f", i, resource_mesh->vertices[i]);
	}*/
	float3 min = resource_mesh->GetAABB().minPoint;
	float3 max= resource_mesh->GetAABB().maxPoint;
	float2 min_p = { min.x,min.y };
	float2 max_p = { max.x,max.y };

	((ComponentTransform2D*)owner->GetComponent(Component_Type::Transform2D))->GetMinandMaxPoints(min_p, max_p);
	float scene_x, scene_y, scene_width, scene_height;
	App->scene_intro->GetSceneDimensions(scene_x, scene_y, scene_width, scene_height);
	LOG("%f, %f", App->input->GetMouseX() - scene_x, App->input->GetMouseY() - scene_y);
	LOG("%f, %f", min_p.x, min_p.y);
	LOG("%f, %f", max_p.x, max_p.y);

}

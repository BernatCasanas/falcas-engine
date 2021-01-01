#include "ComponentUI.h"
#include "Application.h"
#include "ModuleResources.h"
#include "ResourceMesh.h"
#include "ModuleUI.h"
#include "GameObject.h"

ComponentUI::ComponentUI(Component_Type type, GameObject* owner, std::string name, bool has_aabb) : Component(type, owner, name), has_aabb(has_aabb)
{
	resource_mesh = (ResourceMesh*)App->resources->RequestResource(App->UI->mesh_plane_id);
	if (has_aabb) {
		owner->UpdateAABB();
	}
}

ComponentUI::~ComponentUI()
{
	if (resource_mesh != nullptr && !App->resources->isResourcesMapEmpty()) {
		App->resources->FreeResource(resource_mesh);
	}
	resource_mesh = nullptr;
	if (has_aabb) {
		owner->UpdateAABB();
	}
}

void ComponentUI::Render()
{
}

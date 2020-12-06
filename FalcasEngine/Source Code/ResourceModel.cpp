#include "Application.h"
#include "ModuleResources.h"
#include "ResourceModel.h"
#include "ResourceMesh.h"

ResourceModel::ResourceModel(uint ID, Resource_Type type, std::string assets_file) : Resource(ID, type, assets_file)
{
}

ResourceModel::~ResourceModel()
{
	ResourceMesh* mesh;
	for (int i = 0; i < ids_resources_meshes.size(); i++) {
		uint id = ids_resources_meshes[i];
		mesh = (ResourceMesh*)App->resources->GetResource(id);
		delete mesh;
	}
}

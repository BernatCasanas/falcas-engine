#include "ResourceModel.h"

ResourceModel::ResourceModel(uint ID, Resource_Type type, std::string assets_file) : Resource(ID, type, assets_file)
{
}

bool ResourceModel::CleanUp()
{
	textures.clear();
	transform.clear();
	meshes.clear();
	mesh_keys.clear();
	return true;
}

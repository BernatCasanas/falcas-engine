#include "Application.h"
#include "ModuleResources.h"
#include "ResourceModel.h"
#include "ResourceMesh.h"
#include "Importer.h"
#include "FileSystem.h"

ResourceModel::ResourceModel(uint ID, Resource_Type type, std::string assets_file) : Resource(ID, type, assets_file)
{
}

bool ResourceModel::CleanUp()
{
	textures.clear();
	transform.clear();
	meshes.clear();
	return true;
}

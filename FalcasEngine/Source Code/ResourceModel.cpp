#include "Application.h"
#include "ModuleResources.h"
#include "ResourceModel.h"
#include "ResourceMesh.h"
#include "Importer.h"
#include "FileSystem.h"

ResourceModel::ResourceModel(uint ID, Resource_Type type, std::string assets_file) : Resource(ID, type, assets_file)
{
	ModelImporter::Import(this, ID, assets_file);
}

ResourceModel::~ResourceModel()
{
	ResourceMesh* mesh;
	for (int i = 0; i < meshes.size(); i++) {
		uint id = meshes[i];
		mesh = (ResourceMesh*)App->resources->GetResource(id);
		delete mesh;
	}
}

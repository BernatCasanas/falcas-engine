#include "Application.h"
#include "ModuleResources.h"
#include "ResourceModel.h"
#include "ResourceMesh.h"
#include "Importer.h"
#include "FileSystem.h"

ResourceModel::ResourceModel(uint ID, Resource_Type type, std::string assets_file) : Resource(ID, type, assets_file)
{
	char* buffer;
	uint size = App->filesystem->LoadPath((char*)assets_file.c_str(), &buffer);
	ModelImporter::Import(buffer, this, size, ID);
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

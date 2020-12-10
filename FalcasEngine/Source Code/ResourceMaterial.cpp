#include "ResourceMaterial.h"
#include "Application.h"
#include "FileSystem.h"
#include "Importer.h"

ResourceMaterial::ResourceMaterial(uint ID, Resource_Type type, std::string assets_file) : Resource(ID, type, assets_file), full_file_name(assets_file)
{
	file_name = App->filesystem->GetFileName(full_file_name, false);
}

bool ResourceMaterial::CleanUp()
{
	FreeImage(this);
	image_name = 0;
	texture_id = 0;
	std::string full_file_name = "";
	std::string file_name = "";

	int width = 0;
	int height = 0;

	uint size = 0;
	return true;
}

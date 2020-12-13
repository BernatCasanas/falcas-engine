#include "Resource.h"
#include "Application.h"

Resource::Resource(uint ID, Resource_Type type, std::string assets_file) :ID(ID), type(type), assets_file(assets_file)
{
	std::string extension = "";
	switch (type)
	{
	case Resource_Type::Material:
		library_file = "Library/Textures/";
		extension = ".dds";
		break;
	case Resource_Type::Model:
		library_file = "Library/Models/";
		extension = ".falcasmodel";
		break;
	case Resource_Type::Mesh:
		library_file = "Library/Meshes/";
		extension = ".falcasmesh";
		break;
	}
	
	library_file += std::to_string(ID);
	library_file += extension;
}

Resource::~Resource()
{
	
}

uint Resource::GetID() const
{
	return ID;
}

Resource_Type Resource::GetType() const
{
	return type;
}

const char* Resource::GetAssetsFile() const
{
	return assets_file.c_str();
}

const char* Resource::GetLibraryFile() const
{
	return library_file.c_str();
}

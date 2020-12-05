#include "ModuleResources.h"
#include "Application.h"
#include "FileSystem.h"
#include "Importer.h"
#include "ComponentMaterial.h"

ModuleResources::ModuleResources(Application* app, bool start_enabled) : Module(app, start_enabled, "moduleResources")
{
}

bool ModuleResources::Start()
{
	std::vector<std::string> vector_assets_files=App->filesystem->GetAllFiles("Assets/", vector_assets_files, "meta");
	for (int i = 0; i < vector_assets_files.size(); i++) {
		uint id = App->filesystem->GenerateUUID();
		resources.insert(std::pair<uint, std::string>(id, vector_assets_files[i]));
		CreateNewMetaFile(vector_assets_files[i], id);
	}
	return true;
}

void ModuleResources::CreateNewMetaFile(std::string file, uint id)
{
	JsonObj obj;
	obj.AddInt("ID", id);
	obj.AddString("Name", App->filesystem->GetFileName(file,true).c_str());
	char* file_char = new char[file.length() + 1 ];
	strcpy(file_char, file.c_str());
	switch (App->filesystem->GetTypeFile(file_char)) {
	case FILE_TYPE::FBX:
		obj.AddInt("Type", 1);
		//MeshImporter::Import()
		break;
	case FILE_TYPE::DDS:
	case FILE_TYPE::PNG:
	case FILE_TYPE::TGA:
		ComponentMaterial* mat = new ComponentMaterial(nullptr);
		obj.AddInt("Type", 2);
		char* s;
		TextureImporter::Import(mat, file, false, s);
		delete mat;		
		break;
	deafult:
		obj.AddInt("Type", 3);
		break;
	}
	delete[] file_char;
	char* buffer;
	obj.Save(&buffer);
	App->filesystem->SaveInternal((file+".meta").c_str(), buffer, strlen(buffer));
}

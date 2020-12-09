#include "ModuleResources.h"
#include "Application.h"
#include "FileSystem.h"
#include "Importer.h"
#include "ResourceMaterial.h"
#include "ResourceModel.h"

ModuleResources::ModuleResources(Application* app, bool start_enabled) : Module(app, start_enabled, "moduleResources")
{
}

bool ModuleResources::Start()
{
	std::vector<std::string> vector_assets_files;
	vector_assets_files = App->filesystem->GetAllFiles("Assets/", vector_assets_files, "meta", true, false);
	for (int i = 0; i < vector_assets_files.size(); i++) {
		ImportFileToLibrary(vector_assets_files[i], false);
	}
	update_timer.Start();
	return true;
}

update_status ModuleResources::Update(float dt)
{
	if (dt > 0/*BERNAT: HERE WILL GO WHEN THE ENGINE IS OFFLINE!!!!!!!!!!!!!!!!!!*/) {
		if (update_timer.isStoped())
			update_timer.Start();
		if (update_timer.Read() >= 5) {
			UpdateLibrary();
			update_timer.Start();
		}
	}
	else {
		update_timer.Stop();
	}
	return UPDATE_CONTINUE;
}

bool ModuleResources::CleanUp()
{
	for (std::map<uint,Resource*>::iterator it = resources.begin(); it != resources.end(); ) {
		if(it->second!=nullptr)
			FreeResource(it->second);
		delete it->second;
		it=resources.erase(it);
	}
	resources.clear();
	return true;
}

Resource* ModuleResources::GetResource(uint ID) const
{
	return resources.find(ID)->second;
}

Resource* ModuleResources::RequestResource(uint ID)
{
	Resource* resource= GetResource(ID);
	if (resource->referenceCount != 0) {
		resource->referenceCount++;
		return resource;
	}
	switch (resource->GetType())
	{
	case Resource_Type::Material:
	{
		ResourceMaterial* mat = (ResourceMaterial*)resource;
		uint size;
		char* buffer = App->filesystem->ReadPhysFile(mat->GetLibraryFile(), size);
		MaterialImporter::Load(buffer, mat, size);
		mat->referenceCount++;
		break;
	}
	case Resource_Type::Model:
		break;
	case Resource_Type::Mesh:
		break;
	default:
		break;
	}
	
	
	return resource;
}

void ModuleResources::FreeResource(Resource* resource)
{
	resource->referenceCount--;
	if (resource->referenceCount != 0)
		return;
	switch (resource->GetType())
	{
	case Resource_Type::Material:
	{
		ResourceMaterial* mat = (ResourceMaterial*)resource;
		mat->CleanUp();
		break;
	}
	case Resource_Type::Model:
		break;
	case Resource_Type::Mesh:
		break;
	default:
		resource->CleanUp();
		break;
	}

}

void ModuleResources::FreeResource(uint ID)
{
	FreeResource(GetResource(ID));
}


void ModuleResources::UpdateLibrary()
{
	std::vector<std::string> vector_assets_files, vector_assets_meta_files;
	vector_assets_files = App->filesystem->GetAllFiles("Assets/", vector_assets_files, "meta", true, false);
	for (int i = 0; i < vector_assets_files.size(); i++) {
		if (App->filesystem->FileExists(vector_assets_files[i] + ".meta"))
			continue;
		ImportFileToLibrary(vector_assets_files[i], false);
	}
	vector_assets_meta_files = App->filesystem->GetAllFiles("Assets/", vector_assets_meta_files, "meta", false, true);
	
	if (vector_assets_files.size() != vector_assets_meta_files.size()) {
		//File Deleted
		for (int i = 0, difference = vector_assets_meta_files.size() - vector_assets_files.size(); i < vector_assets_meta_files.size() && difference>0; i++) {
			std::string meta_file = vector_assets_meta_files[i];
			meta_file=meta_file.substr(0,meta_file.find_last_of('.'));
			if (!App->filesystem->FileExists(meta_file.c_str())) {
				char* buffer;
				App->filesystem->Load((vector_assets_meta_files[i]).c_str(), &buffer);
				JsonObj meta_id(buffer);
				uint id = meta_id.GetInt("ID");
				DeleteResourceLibrary(resources.find(id)->second);
				delete resources.find(id)->second;
				resources.erase(id);
				delete[] buffer;
				difference--;
			}
		}
		vector_assets_meta_files.clear();
		vector_assets_meta_files = App->filesystem->GetAllFiles("Assets/", vector_assets_meta_files, "meta", false, true);
	}
	for (int i = 0; i < vector_assets_meta_files.size(); i++) {
		char* buffer;
		App->filesystem->Load((vector_assets_meta_files[i]).c_str(), &buffer);
		JsonObj mod_time(buffer);
		int mod = mod_time.GetInt("Date");
		int mod2 = App->filesystem->GetLastModificationTime(vector_assets_files[i]);
		if (mod_time.GetInt("Date") != App->filesystem->GetLastModificationTime(vector_assets_files[i])) {
			CreateNewMetaFile(vector_assets_files[i], mod_time.GetInt("ID"));
		}
		delete[] buffer;
	}
}

void ModuleResources::ImportFileToLibrary(std::string file, bool drag_and_drop)
{
	if (drag_and_drop) {
		file=App->filesystem->CopyPhysFile(file);
	}
	uint id;
	if (App->filesystem->FileExists(file + ".meta")) {
		char* buffer;
		App->filesystem->Load((file + ".meta").c_str(), &buffer);
		JsonObj meta_file(buffer);
		id = meta_file.GetInt("ID");
		if (meta_file.GetInt("Date") == App->filesystem->GetLastModificationTime(file)) {
			UpdateMetaFile(file, id, buffer);
		}
		else {
			delete[] buffer;
		}
	}
	else {
		id = App->filesystem->GenerateUUID(); 
		CreateNewMetaFile(file, id);
	}
	resources.insert(std::pair<uint, Resource*>(id, CreateNewResource(id,file)));
}

void ModuleResources::UpdateMetaFile(std::string meta_file, uint id, char* buffer)
{
	JsonObj obj_meta_existing(buffer);
	JsonObj obj;
	obj.AddInt("ID", id);
	obj.AddString("Name", obj_meta_existing.GetString("Name"));
	obj.AddInt("Type", obj_meta_existing.GetInt("Type"));
	obj.AddInt("Date", obj_meta_existing.GetInt("Date"));
	delete[] buffer;
	obj.Save(&buffer);
	App->filesystem->SaveInternal((meta_file + ".meta").c_str(), buffer, strlen(buffer));
	delete[] buffer;
}

void ModuleResources::DeleteResourceLibrary(Resource* resource)
{
	if (resource->GetType() == Resource_Type::Model) {
		ResourceModel* model = (ResourceModel*)resource;
		for (int i = 0; i < model->meshes.size(); i++) {
			App->filesystem->DeleteAFile(GetResource(model->meshes[i])->GetLibraryFile());
		}
	}
	App->filesystem->DeleteAFile(resource->GetLibraryFile());
	std::string assets_file = resource->GetAssetsFile();
	App->filesystem->DeleteAFile(assets_file  + ".meta");
}

Resource* ModuleResources::CreateNewResource(uint ID, std::string assets_file)
{
	char* file_char = new char[assets_file.length() + 1];
	strcpy(file_char, assets_file.c_str());
	Resource_Type res_type=Resource_Type::None;
	Resource* resource = nullptr;
	switch (App->filesystem->GetTypeFile(file_char))
	{
	case FILE_TYPE::FBX:
		res_type = Resource_Type::Model;
		resource = new ResourceModel(ID, res_type, assets_file);
		break;
	case FILE_TYPE::PNG:
	case FILE_TYPE::TGA:
		res_type = Resource_Type::Material;
		resource = (Resource*)new ResourceMaterial(ID, res_type, assets_file);
		break;
	}
	delete[] file_char;
	
	//Resource* resource = new Resource(ID, res_type, assets_file);
	return resource;
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
		ImportFBX(file, id);
		//MeshImporter::Import()
		break;
	case FILE_TYPE::PNG:
	case FILE_TYPE::TGA:
		obj.AddInt("Type", 2);
		MaterialImporter::Import(file, id);
		break;
	deafult:
		obj.AddInt("Type", 3);
		break;
	}
	delete[] file_char;
	obj.AddInt("Date", App->filesystem->GetLastModificationTime(file));
	char* buffer;
	obj.Save(&buffer);
	App->filesystem->SaveInternal((file+".meta").c_str(), buffer, strlen(buffer));
	delete[] buffer;
}

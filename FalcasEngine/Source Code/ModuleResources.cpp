#include "ModuleResources.h"
#include "Application.h"
#include "FileSystem.h"
#include "Importer.h"
#include "ResourceMaterial.h"
#include "ResourceModel.h"
#include "ResourceMesh.h"
#include "aClock.h"
#include "ModuleSceneIntro.h"

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
	if (Time::gameTimer.stopped) {
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
		delete it->second;
		it=resources.erase(it);
	}
	resources.clear();
	return true;
}

Resource* ModuleResources::GetResource(uint ID)
{
	std::map<uint, Resource*>::iterator it = resources.find(ID);
	if (it == resources.end())
		return nullptr;
	return it->second;
}

Resource* ModuleResources::RequestResource(uint ID)
{
	if (ID == 0) return nullptr;
	Resource* resource= GetResource(ID);
	if (resource->referenceCount != 0) {
		resource->referenceCount++;
		return resource;
	}
	switch (resource->GetType())
	{
	case Resource_Type::Material:
	{
		uint size;
		ResourceMaterial* mat = (ResourceMaterial*)resource;
		char* buffer = App->filesystem->ReadPhysFile(mat->GetLibraryFile(), size);
		if (buffer == "") break;
		MaterialImporter::Load(buffer, mat, size);
		mat->referenceCount++;
		break;
	}
	case Resource_Type::Model: {
		ResourceModel* model = (ResourceModel*)resource;
		uint size;
		char* buffer = App->filesystem->ReadPhysFile(model->GetLibraryFile(), size);
		if (buffer == "") break;
		ModelImporter::Load(buffer, model);
		model->referenceCount++;
		break;
	}
	case Resource_Type::Mesh:
	{
		ResourceMesh* mesh = (ResourceMesh*)resource;
		uint size;
		char* buffer = App->filesystem->ReadPhysFile(mesh->GetLibraryFile(), size);
		if (buffer == "") break;
		MeshImporter::Load(buffer, mesh);
		mesh->referenceCount++;
		break;
	}
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
	{
		ResourceModel* model = (ResourceModel*)resource;
		model->CleanUp();
		break;
	}
	case Resource_Type::Mesh:
	{
		ResourceMesh* mesh = (ResourceMesh*)resource;
		mesh->CleanUp();
		break;
	}
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
	
	bool different;
	do {
		different = false;
		if (vector_assets_files.size() != vector_assets_meta_files.size()) {
			different = true;
			//File Deleted
			for (int i = 0, difference = vector_assets_meta_files.size() - vector_assets_files.size(); i < vector_assets_meta_files.size() && difference>0; i++) {
				std::string meta_file = vector_assets_meta_files[i];
				meta_file = meta_file.substr(0, meta_file.find_last_of('.'));
				if (!App->filesystem->FileExists(meta_file.c_str())) {
					char* buffer;
					App->filesystem->Load((vector_assets_meta_files[i]).c_str(), &buffer);
					JsonObj meta_id(buffer);
					uint id = meta_id.GetInt("ID");
					DeleteResourceLibrary(resources.find(id)->second);
					delete resources.find(id)->second;
					resources.erase(id);
					delete[] buffer;
					meta_id.CleanUp();
					difference--;
				}
			}
			vector_assets_meta_files.clear();
			vector_assets_meta_files = App->filesystem->GetAllFiles("Assets/", vector_assets_meta_files, "meta", false, true);
		}
	} while (different == true);
	for (int i = 0; i < vector_assets_meta_files.size(); i++) {
		char* buffer;
		App->filesystem->Load((vector_assets_meta_files[i]).c_str(), &buffer);
		JsonObj mod_time(buffer);
		int mod = mod_time.GetInt("Date");
		int mod2 = App->filesystem->GetLastModificationTime(vector_assets_files[i]);
		if (mod_time.GetInt("Date") != App->filesystem->GetLastModificationTime(vector_assets_files[i])) {
			CreateNewMetaFile(vector_assets_files[i], mod_time.GetInt("ID"));
		}
		mod_time.CleanUp();
		delete[] buffer;
	}
}

void ModuleResources::ImportFileToLibrary(std::string file, bool drag_and_drop)
{
	if (drag_and_drop) {
		file=App->filesystem->CopyPhysFile(file);
	}
	uint id;
	int type = 0;
	if (App->filesystem->FileExists(file + ".meta")) {
		char* buffer;
		App->filesystem->Load((file + ".meta").c_str(), &buffer);
		JsonObj meta_file(buffer);
		id = meta_file.GetInt("ID");
		type = meta_file.GetInt("Type");
		if (meta_file.GetInt("Date") == App->filesystem->GetLastModificationTime(file)) {
			delete[] buffer;
		}
		else {
			Resource* resource= CreateNewResource(id, file);
			DeleteResourceLibrary(resource);
			delete resource;
			CreateNewMetaFile(file, id);
			delete[] buffer;
		}
		meta_file.CleanUp();
	}
	else {
		id = App->filesystem->GenerateUUID(); 
		CreateNewMetaFile(file, id);
	}
	resources.insert(std::pair<uint, Resource*>(id, CreateNewResource(id,file)));
	if (type != 1)
		return;
	ResourceModel* model = (ResourceModel*)RequestResource(id);
	for (std::map<uint,uint>::iterator it = model->meshes.begin(); it != model->meshes.end(); ++it) {
		CreateNewMeshResource(it->first, file);
	}
	FreeResource(id);
	
}



void ModuleResources::DeleteResourceLibrary(Resource* resource)
{
	if (resource->GetType() == Resource_Type::Model) {
		ResourceModel* model = (ResourceModel*)RequestResource(resource->GetID());
		App->scene_intro->resources_model_to_delete.push_back(model);
		for (std::map<uint, uint>::iterator it = model->meshes.begin(); it != model->meshes.end(); ++it) {
			DeleteMeshResource((ResourceMesh*)GetResource(it->first));
		}
	}
	else if (resource->GetType() == Resource_Type::Material)
		App->scene_intro->resources_material_to_delete.push_back((ResourceMaterial*)resource);
	App->filesystem->DeleteAFile(resource->GetLibraryFile());
	std::string assets_file = resource->GetAssetsFile();
	App->filesystem->DeleteAFile(assets_file  + ".meta");
}

void ModuleResources::DeleteMeshResource(ResourceMesh* resource)
{
	App->scene_intro->resources_mesh_to_delete.push_back(resource);
	App->filesystem->DeleteAFile(resource->GetLibraryFile());
	int id = resource->GetID();
	delete resource;
	resources.erase(id);
}

Resource* ModuleResources::CreateNewResource(uint ID, std::string assets_file)
{
	Resource_Type res_type=Resource_Type::None;
	Resource* resource = nullptr;
	switch (App->filesystem->GetTypeFile(assets_file))
	{
	case FILE_TYPE::FBX:
		res_type = Resource_Type::Model;
		resource = (Resource*)new ResourceModel(ID, res_type, assets_file);
		break;
	case FILE_TYPE::PNG:
	case FILE_TYPE::TGA:
		res_type = Resource_Type::Material;
		resource = (Resource*)new ResourceMaterial(ID, res_type, assets_file);
		break;
	}
	
	return resource;
}

void ModuleResources::CreateNewMetaFile(std::string file, uint id)
{
	JsonObj obj;
	obj.AddInt("ID", id);
	obj.AddString("Name", App->filesystem->GetFileName(file,true).c_str());
	switch (App->filesystem->GetTypeFile(file)) {
	case FILE_TYPE::FBX:
		obj.AddInt("Type", 1);
		ModelImporter::Import(file, id);
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
	obj.AddInt("Date", App->filesystem->GetLastModificationTime(file));
	char* buffer;
	obj.Save(&buffer);
	App->filesystem->SaveInternal((file+".meta").c_str(), buffer, strlen(buffer));
	delete[] buffer;
	obj.CleanUp();
}

void ModuleResources::CreateNewMeshResource(uint ID, std::string model_assets_file)
{

	Resource* resource = (Resource*)new ResourceMesh(ID, Resource_Type::Mesh, model_assets_file);

	resources.insert(std::pair<uint, Resource*>(ID, resource));
}

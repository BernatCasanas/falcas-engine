#pragma once
#include "Module.h"
#include "Timer.h"
#include <map>

class Resource;
class ResourceMesh;

class ModuleResources : public Module
{
public:
	ModuleResources(Application* app, bool start_enabled = true);
	~ModuleResources() {};

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	Resource* GetResource(uint ID);
	Resource* RequestResource(uint ID);
	void FreeResource(Resource* resource);
	void FreeResource(uint ID);

	void UpdateLibrary();
	void ImportFileToLibrary(std::string file, bool drag_and_drop);
	void DeleteResourceLibrary(Resource* resource);
	void DeleteMeshResource(ResourceMesh* resource);

	void CreateNewMetaFile(std::string file, uint id);
	void CreateNewMeshResource(uint ID, std::string model_assets_file);
private:
	Resource* CreateNewResource(uint ID, std::string assets_file);
	std::map<uint, Resource*> resources;
	Timer update_timer;
};
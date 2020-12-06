#pragma once
#include "Module.h"
#include "Timer.h"
#include <map>

class Resource;

class ModuleResources : public Module
{
public:
	ModuleResources(Application* app, bool start_enabled = true);
	~ModuleResources() {};

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	Resource* GetResource(uint ID) const;
	Resource* RequestResource(uint ID);

	void UpdateLibrary();
	void ImportFileToLibrary(std::string file, bool drag_and_drop);
	void DeleteResourceLibrary(Resource* resource);

private:
	void CreateNewMetaFile(std::string file, uint id);
	void UpdateMetaFile(std::string meta_file, uint id, char* buffer);
private:
	Resource* CreateNewResource(uint ID, std::string assets_file);
	std::map<uint, Resource*> resources;
	Timer update_timer;
};
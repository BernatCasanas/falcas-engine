#pragma once
#include "Module.h"
#include <map>

class Resource;

class ModuleResources : public Module
{
public:
	ModuleResources(Application* app, bool start_enabled = true);
	~ModuleResources() {};

	bool Start();
	update_status Update(float dt) { return UPDATE_CONTINUE; };
	bool CleanUp() { return true; };

	void CreateNewMetaFile(std::string file, uint id);
	void UpdateMetaFile(std::string meta_file, uint id, char* buffer);
private:
	std::map<uint, std::string> resources;
};
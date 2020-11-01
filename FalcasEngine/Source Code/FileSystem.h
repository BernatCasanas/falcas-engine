#pragma once

#ifndef _FILE_SYSTEM_
#define _FILE_SYSTEM_

#include "GameObject.h"
#include "Component.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ModuleSceneIntro.h"
#include "Application.h"
#include "External Libraries/PhysFS/include/physfs.h"
#include "Globals.h"
#include "External Libraries/SDL/include/SDL.h"

#pragma comment(lib, "PhysFS/libx86/physfs.lib")


class FileSystem : public Module {
public:
	FileSystem(Application* app, bool start_enabled = true);
	~FileSystem();

	bool Start();
	update_status Update(float dt) { return UPDATE_CONTINUE; };
	bool CleanUp() { return true; };

public:
	uint Load(const char* path, char** buffer) const;
};


#endif // !_FILE_SYSTEM_

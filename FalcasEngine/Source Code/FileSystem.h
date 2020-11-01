#pragma once

#ifndef _FILE_SYSTEM_
#define _FILE_SYSTEM_


#include "Module.h"
#include "Globals.h"


class Application;
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

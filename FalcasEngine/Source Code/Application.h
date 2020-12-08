#pragma once

#ifndef __Application_H__
#define __Application_H__

#include "Globals.h"
#include "Timer.h"
#include <vector>

class ModuleWindow;
class ModuleInput;
class ModuleSceneIntro;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleCentralEditor;
class Console;
class Module;
class FileSystem;
class ModuleResources;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleCentralEditor* central_editor;
	Console* console;
	FileSystem* filesystem;
	ModuleResources* resources;
	bool console_active;

private:

	float	dt;
	float	fps;
	std::vector<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();



private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;

#endif //__Application_H__
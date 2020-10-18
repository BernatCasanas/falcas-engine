#include "Application.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleCentralEditor.h"
#include "Console.h"
#include "Asap.h"

Application::Application()
{
	console_active = true;
	console = new Console();
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	physics = new ModulePhysics3D(this);
	central_editor = new ModuleCentralEditor(this);
	asap = new Asap(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);
	AddModule(asap);
	
	// Scenes
	AddModule(scene_intro);


	AddModule(central_editor);
	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	delete console;
	Module* item;

	while(list_modules.empty() == false)
	{
		item = list_modules.back();
		delete item;
		list_modules.pop_back();
	}
}

bool Application::Init()
{
	bool ret = true;
	Module* item;

	// Call Init() in all modules

	int size = list_modules.size();
	for (int i = 0; i < size && ret==true; i++) {
		ret = list_modules.at(i)->Init();
	}
	

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");

	for (int i = 0; i < size && ret == true; i++) {
		ret = list_modules.at(i)->Start();
	}
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	Module* item;


	int size = list_modules.size();
	for (int i = 0; i < size && ret == UPDATE_CONTINUE; i++) {
		ret = list_modules.at(i)->PreUpdate(dt);
	}

	for (int i = 0; i < size && ret == UPDATE_CONTINUE; i++) {
		ret = list_modules.at(i)->Update(dt);
	}

	for (int i = 0; i < size && ret == UPDATE_CONTINUE; i++) {
		ret = list_modules.at(i)->PostUpdate(dt);
	}


	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	Module* item;


	int size = list_modules.size();
	for (int i = 0; i < size && ret == true; i++) {
		ret = list_modules.at(i)->CleanUp();
	}

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}
#include "Application.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleCentralEditor.h"
#include "Console.h"
#include "ModuleResources.h"
#include "FileSystem.h"
#include "ModuleUI.h"
#include "Json.h"
#include "aClock.h"
#include "External Libraries/SDL/include/SDL.h"

Application::Application()
{
	console_active = true;
	console = new Console();
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	central_editor = new ModuleCentralEditor(this);
	filesystem = new FileSystem(this);
	resources = new ModuleResources(this);
	UI = new ModuleUI(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	inGame = false;

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(filesystem);
	AddModule(resources);
	
	// Scenes
	AddModule(scene_intro);
	AddModule(UI);


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

	Time::Init();
	App->filesystem->SetWritenDir();
	App->filesystem->CreateFolders();

	char* buffer = nullptr;
	
	JsonObj config = App->filesystem->GenerateConfigContent();
	config.Save(&buffer);
	App->filesystem->SaveInternal("Config/config.json", buffer, strlen(buffer));
	uint sizeConfig = App->filesystem->LoadPath("Config/config.json",&buffer);
	JsonArray arrayModules(config.GetArray("configModules"));

	// Call Init() in all modules

	int size = list_modules.size();
	for (int i = 0; i < size && ret==true; i++) {
		ret = list_modules[i]->LoadConfig(arrayModules.GetObjectInArray(list_modules[i]->name));
		ret = list_modules[i]->Init();
	}
	

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");

	for (int i = 0; i < size && ret == true; i++) {
		ret = list_modules[i]->Start();
	}
	
	Time::realTimer.deltaTimer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)Time::realTimer.deltaTimer.Read() / 1000;
	fps = 1.0f / dt;

	Time::realTimer.Step();
	Time::gameTimer.Step();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	Uint32 last_frame_ms = Time::realTimer.deltaTimer.Read();
	if (last_frame_ms < 16.66)
	{
		SDL_Delay(16.66 - last_frame_ms);
	}

	Time::numFrames++;
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	Module* item;



	int size = list_modules.size();
	for (int i = 0; i < size && ret == UPDATE_CONTINUE; i++) {
		ret = list_modules[i]->PreUpdate(dt);
	}

	for (int i = 0; i < size && ret == UPDATE_CONTINUE; i++) {
		ret = list_modules[i]->Update(dt);
	}

	for (int i = 0; i < size && ret == UPDATE_CONTINUE; i++) {
		ret = list_modules[i]->PostUpdate(dt);
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
		ret = list_modules[i]->CleanUp();
	}

	return ret;
}

void Application::StartGame()
{
	if (!inGame)
	{
		inGame = true;
		Time::gameTimer.Start();
		App->central_editor->SaveScene("temp");
	}
}

void Application::PauseGame()
{
	if (!isPaused()) {
		Time::gameTimer.Pause();
	}
}

void Application::ResumeGame()
{
	if (isPaused()) {
		Time::gameTimer.Resume();
	}
}

void Application::StopGame()
{
	if (inGame)
	{
		inGame = false;
		Time::gameTimer.Stop();
		App->central_editor->LoadScene("Library/Scenes/temp.scenefalcas");
	}
}

bool Application::isPlaying()
{
	bool ret;
	Time::gameTimer.started ? ret = true : ret = false;
	return ret;
}

bool Application::isPaused()
{
	bool ret;
	Time::gameTimer.paused ? ret = true : ret = false;
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}
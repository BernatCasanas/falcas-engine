#pragma once
#include "Globals.h"
#include "Json.h"
class ComponentUI;
class Application;
class Module
{
private :
	bool enabled;

public:
	char* name;
	Application* App;

	Module(Application* parent, bool start_enabled = true, char* n = "") : App(parent)
	{
		name = n;
	}

	virtual ~Module()
	{}

	virtual bool LoadConfig(JsonObj& obj) 
	{
		return true;
	}

	virtual bool SaveConfig(JsonObj& obj)
	{
		return true;
	}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void OnTriggered(ComponentUI* component_ui) {}
};
#pragma once
#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "External Libraries/SDL/include/SDL_video.h"

class Application;
class SDL_Window;
class SDL_Surface;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool LoadConfig(JsonObj* obj);
	bool CleanUp();

	void SetTitle(const char* title);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;


	SDL_GLContext gl_context;

	const char* glsl_version;

	int height, width;
	bool fullscreen, resizeable, border;
	char* appName;
};

#endif // __ModuleWindow_H__
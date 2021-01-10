
#include "Application.h"
#include "ModuleWindow.h"
#include "External Libraries/SDL/include/SDL.h"
#include "ComponentUI.h"
#include "ComponentCheckbox.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled, "moduleWindow")
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		// Decide GL+GLSL versions
		#if __APPLE__
				// GL 3.2 Core + GLSL 150
				const char* glsl_version = "#version 150";
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		#else
				// GL 3.0 + GLSL 130
				glsl_version = "#version 130";
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		#endif



		SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

		SDL_WindowFlags resize_flag = resizeable ? SDL_WINDOW_RESIZABLE : (SDL_WindowFlags)0x00000000;
		SDL_WindowFlags fullscreen_flag = fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : (SDL_WindowFlags)0x00000000;
		SDL_WindowFlags border_flag = !border ? SDL_WINDOW_BORDERLESS : (SDL_WindowFlags)0x00000000;

		

		//Use OpenGL 2.1	
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		window = SDL_CreateWindow(appName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags | resize_flag | fullscreen_flag| border_flag);
		gl_context = SDL_GL_CreateContext(window);
		SDL_GL_MakeCurrent(window, gl_context);
		SDL_GL_SetSwapInterval(1); // Enable vsync

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}
	
	return ret;
}

bool ModuleWindow::LoadConfig(JsonObj& obj)
{
	appName = (char*)obj.GetString("appName");
	height = obj.GetInt("height");
	width = obj.GetInt("width");
	resizeable = obj.GetBool("resizeable");
	fullscreen = obj.GetBool("fullscreen");
	border = obj.GetBool("border");
	return true;
}


// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_GL_DeleteContext(gl_context);
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::OnTriggered(ComponentUI* component_ui)
{
	if (component_ui->type == Component_Type::Checkbox) {
		ComponentCheckbox* check = (ComponentCheckbox*)component_ui;
		SDL_GL_SetSwapInterval(!check->IsActive());
	}
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}
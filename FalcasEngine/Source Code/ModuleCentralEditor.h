#ifndef __ModuleCentralEditor_H__
#define __ModuleCentralEditor_H__

#include "Module.h"
#include "External Libraries/SDL/include/SDL.h"
#include <list>
#include "External Libraries/ImGui/imgui.h"

class Application;
class ImVec4;

class ModuleCentralEditor : public Module
{
public:

	ModuleCentralEditor(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleCentralEditor();

	bool Init();
	bool CleanUp();

	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);


	bool ProcessEvents(SDL_Event event);



public:

	bool show_demo_window;
	bool show_another_window;
	bool show_example;
	bool show_about;
	bool show_configuration;
	bool show_console;

	float progress;
	float progress2;
	float progress3;
	float progress4;

	std::list<float> fr_arr;
	std::list<float> ms_arr;

	ImGuiTextBuffer  input_list;
	bool need_scroll = false;
};

#endif // __ModuleCentralEditor_H__
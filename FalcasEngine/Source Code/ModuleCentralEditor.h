#pragma once
#ifndef __ModuleCentralEditor_H__
#define __ModuleCentralEditor_H__

#include "Module.h"
#include <list>
#include <vector>
#include "External Libraries/ImGui/imgui_impl_sdl.h"


class Application;
class GameObject;

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

	void Draw();


	bool ProcessEvents(SDL_Event event);

	void HierarchyRecursiveTree(GameObject* game_object, static int selected, static ImGuiTreeNodeFlags base_flags, int &node_clicked);


	void CreateDock();

public:

	bool show_demo_window;
	bool show_another_window;
	bool show_example;
	bool show_about;
	bool show_configuration;
	bool show_console;
	bool show_openglOptions;
	bool show_hierarchy;
	bool show_inspector;

	bool depth, cullface, lighting, colorMaterial, texture, ambient, stencil, wireframe, normals_v, normals_f;

	float progress;
	float progress2;
	float progress3;
	float progress4;

	std::list<float> fr_arr;
	std::list<float> ms_arr;

	std::vector<char*> console_logs;

	ImGuiTextBuffer  input_list;
	bool need_scroll = false;

	bool wantToExit = false;
};

#endif // __ModuleCentralEditor_H__
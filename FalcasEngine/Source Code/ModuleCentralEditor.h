#pragma once
#ifndef __ModuleCentralEditor_H__
#define __ModuleCentralEditor_H__

#include "Module.h"
#include <list>
#include <vector>
#include <string>


enum class Shape {
    Cube,
    Rectangular_Prism,
    Triangular_Pyramid,
    Square_Pyramid,
    Rectangular_Pyramid,
	Cilinder,
	Cone,
	Sphere,
	Plane
};

class Application;
class GameObject;
typedef int ImGuiTreeNodeFlags; 
union SDL_Event;

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

	bool LoadFile();
	bool SaveScene();

	bool ProcessEvents(SDL_Event event);

	void FilesRecursiveTree(const char* path);
	void HierarchyRecursiveTree(GameObject* game_object, static ImGuiTreeNodeFlags base_flags, int &node_clicked);
	void SaveAllGameObjectsTree(GameObject* game_object, JsonArray arr);

	void SelectObject(GameObject* game_obj);

	void CreateDock();
	void CreateShape(Shape shape, std::string name);

public:

	bool show_demo_window = true;
	bool show_another_window = false;
	bool show_example = false;
	bool show_about = false;
	bool show_configuration = false;
	bool show_console = true;
	bool show_openglOptions = false;
	bool show_hierarchy = true;
	bool show_inspector = true;

	bool cullface = false;
	bool colorMaterial = false;
	bool ambient = false;
	bool stencil = false;
	bool wireframe = false;
	bool normals_v = false;
	bool normals_f = false;
	bool aabbs = false;
	bool depth = true;
	bool lighting = true;
	bool texture = true;
	bool frustums = false;
	bool loading_file = false;

	float progress;
	float progress2;
	float progress3;
	float progress4;

	std::list<float> fr_arr;
	std::list<float> ms_arr;

	std::vector<char*> console_logs;

	std::vector <char*> input_list;
	bool need_scroll = false;

	bool wantToExit = false;

private:
	bool selected_through_screen = false;
	int id_popup_shown = -1;
	bool is_popup_shown = false;
	bool delete_game_object = false;
	std::vector<int> ids_of_parents_selected;
	char selected_file[100] = "";
};

#endif // __ModuleCentralEditor_H__
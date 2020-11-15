#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleCentralEditor.h"
#include "ModuleCamera3D.h"
#include "Console.h"
#include "GameObject.h"
#include "External Libraries/MathGeoLib/include/Math/Quat.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include <gl/GL.h>
#include "Shape.h"
#include "FileSystem.h"
#include "Importer.h"
#include "External Libraries/ImGui/imgui_impl_sdl.h"
#include "ModuleRenderer3D.h"





ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	DevilInit();

	id_gameobject = -1;
	root = new GameObject(id_gameobject, "Grid", nullptr, { 0,0,0 }, Quat::identity, { 1,1,1 });
	id_gameobject++;
	ComponentMesh* mesh = (ComponentMesh*)root->CreateComponent(Component_Type::Mesh);
	mesh->grid = true;
	CreateGrid(mesh->num_vertices, mesh->num_indices, mesh->indices, mesh->vertices, { 0,0,0 }, { 500,1,500 });
	mesh->Initialization();
	camera = (ComponentCamera*)CreateGameObject("Camera", root)->CreateComponent(Component_Type::Camera);
	App->renderer3D->camera_culling = camera;
	id_gameobject++;
	game_object_selected = nullptr;
	int num = 0;

	std::string path = "Assets/BakerHouse/BakerHouse.fbx";
	ImportFBX(path);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	delete root;
	game_object_selected = nullptr;
	DevilCleanUp();

	return true;
}

void ModuleSceneIntro::Draw(GLuint tex)
{
	ImGui::Begin("Scene");

	ImVec2 windowSize = ImGui::GetWindowSize();
	width = windowSize.x;
	height = windowSize.y;
	ImGui::Image((ImTextureID)tex, windowSize, ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
}

bool ModuleSceneIntro::GetDimensionsWindow(float& width, float& height)
{
	if (width == this->width && height == this->height)
		return false;
	width = this->width;
	height = this->height;
	return true;
}

int ModuleSceneIntro::GetID()
{
	return id_gameobject;
}


// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	root->Update();

	
	if (App->central_editor->wireframe) {
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
	}
	

	return UPDATE_CONTINUE;
}

GameObject* ModuleSceneIntro::CreateGameObject(std::string name, GameObject* parent)
{
	GameObject* game_object = nullptr;
	name = CheckNameGameObject(name);
	game_object = new GameObject(App->scene_intro->id_gameobject, name, parent);
	if (game_object != nullptr) {
		App->scene_intro->id_gameobject++;
		if (parent != nullptr) {
			parent->children.push_back(game_object);
		}

	}
	return game_object;
}

GameObject* ModuleSceneIntro::CreateGameObject(float3 position, Quat rotation, float3 size, std::string name, GameObject* parent)
{
	GameObject* game_object = nullptr;
	name = CheckNameGameObject(name);
	game_object = new GameObject(App->scene_intro->id_gameobject, name, parent, position, rotation, size);
	if (game_object != nullptr) {
		App->scene_intro->id_gameobject++;
		if (parent != nullptr) {
			parent->children.push_back(game_object);
		}
	}
	return game_object;
}




GameObject* ModuleSceneIntro::SearchGameObject(int id, GameObject* game_obj)
{
	GameObject* game_object = nullptr;
	if (game_obj->id == id) {
		return game_obj;
	}
	for (int i = 0; i < game_obj->children.size() && game_object == nullptr; i++) {
		game_object = SearchGameObject(id, game_obj->children[i]);
	}
	return game_object;
}

bool ModuleSceneIntro::IsGameObjectNameRepeated(std::string name, GameObject* game_obj)
{
	bool ret = false;
	if (game_obj->name == name) {
		return true;
	}
	for (int i = 0; i < game_obj->children.size() && ret == false; i++) {
		ret = IsGameObjectNameRepeated(name, game_obj->children[i]);
	}
	return ret;
}

std::string ModuleSceneIntro::CheckNameGameObject(std::string name, bool numbered)
{
	bool stop = IsGameObjectNameRepeated(name,App->scene_intro->root);
	if (stop == true) {
		if (numbered) {
			name.pop_back();
			int digits = 0;
			int number = 0;
			stop = false;
			do {
				if (isdigit(name.back())) {
					number += (name.back() - '0') * (pow(10, digits));
					name.pop_back();
					digits++;
				}
				else {
					stop=true;
				}
			} while (stop == false);
			number++;
			if (number - pow(10, digits) >= 0) {
				digits++;
			}
			for (int i = digits - 1; i > 0; i--) {
				if (number > (pow(10, i)) - 1) {
					int n = number / pow(10, i);
					name.push_back(n + '0');
					number -= n * (pow(10, i));
				}
				else {
					name.push_back(0 + '0');
				}
			}
			name.push_back(number + '0');
			name.push_back(')');
		}
		else {
			name.push_back(' ');
			name.push_back('(');
			name.push_back('1');
			name.push_back(')');
		}
		name = CheckNameGameObject(name, true);
	}
	return name;
}

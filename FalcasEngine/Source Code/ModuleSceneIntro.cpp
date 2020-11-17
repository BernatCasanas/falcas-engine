#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleCentralEditor.h"
#include "ModuleCamera3D.h"
#include "Console.h"
#include "GameObject.h"
#include "External Libraries/MathGeoLib/include/Math/Quat.h"
#include "External Libraries/MathGeoLib/include/Geometry/LineSegment.h"
#include "External Libraries/MathGeoLib/include/Geometry/Triangle.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
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
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("Scene");
	ImVec2 win = ImGui::GetContentRegionMax();
	LOG("%f, %f", win.x, win.y);
	ImVec2 windowSize = ImGui::GetWindowSize();
	windowSize.y;// -= 35;
	ImVec2 windowPos = ImGui::GetWindowPos();
	windowPos.x;// -= 8;
	windowPos.y;// ; -= 8;
	x = windowPos.x;
	y = windowPos.y;
	width = windowSize.x;
	height = windowSize.y;
	ImVec2 min = ImGui::GetWindowContentRegionMin();
	ImVec2 max = ImGui::GetWindowContentRegionMax();
	min.x += x;
	min.y += y;
	max.x += x;
	max.y += y;
	windowSize.y -= min.y-windowPos.y;
	ImGui::GetForegroundDrawList()->AddRect(min, max, IM_COL32(255, 255, 0, 255));
	ImGui::Image((ImTextureID)tex, windowSize, ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
	ImGui::PopStyleVar();

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

void ModuleSceneIntro::GetSceneDimensions(float& x, float& y, float& width, float& height)
{
	x = this->x;
	y = this->y;
	width = this->width;
	height = this->height;
}

void ModuleSceneIntro::SelectGameObjectWithRay(LineSegment ray)
{
	std::map<float, GameObject*> list_of_possible;
	list_of_possible = CheckIfGameObjectIsSelectable(root, list_of_possible, ray);
	if (list_of_possible.size() == 0) {
		game_object_selected = nullptr;
		App->central_editor->SelectObject(game_object_selected);
		return;
	}
	GameObject* game_obj_selected=nullptr;
	float distance = -1;
	for (std::map<float, GameObject*>::iterator it = list_of_possible.begin(); it != list_of_possible.end(); ++it) {
		bool last_element = false;
		if (distance >= 0 && distance < it->first)
			break;
		if (++it == list_of_possible.end()) {
			last_element = true;
		}
		--it;
		ComponentTransform* trans = (ComponentTransform*)it->second->GetComponent(Component_Type::Transform);
		LineSegment ray_local = ray;
		ray_local.Transform(trans->GetGlobalMatrix().Inverted());
		ComponentMesh* mesh = (ComponentMesh*)it->second->GetComponent(Component_Type::Mesh);
		for (int i = 0; i < mesh->num_indices; i+=3) {
			Triangle tri;
			tri.a = float3(&mesh->vertices[mesh->indices[i]*3]);
			tri.b = float3(&mesh->vertices[mesh->indices[i+1] * 3]);
			tri.c = float3(&mesh->vertices[mesh->indices[i+2] * 3]);
			float3 point;
			float distance_local;
			if (ray_local.Intersects(tri, &distance_local, &point)) {
				distance_local *= 1000;
				if (distance_local < distance || distance < 0) {
					distance = distance_local;
					game_obj_selected = it->second;
					if (last_element)
						break;
				if (distance_local <= it->first)
					break;
				}
			}
		}
	}
	game_object_selected = game_obj_selected;
	App->central_editor->SelectObject(game_object_selected);
}

std::map<float, GameObject*> ModuleSceneIntro::CheckIfGameObjectIsSelectable(GameObject* game_obj, std::map<float, GameObject*> map, LineSegment ray)
{
	if (game_obj->id >= 0 && game_obj->HasComponentType(Component_Type::Mesh)) {
		if (ray.Intersects(game_obj->GetAABB())) {
			float distance = Length(game_obj->GetAABB().ClosestPoint(ray.a) - ray.a);
			map.insert(std::pair<float, GameObject*>(distance, game_obj));
		}
	}
	for (int i = 0; i < game_obj->children.size(); i++) {
		map=CheckIfGameObjectIsSelectable(game_obj->children[i], map, ray);
	}
	return map;
}

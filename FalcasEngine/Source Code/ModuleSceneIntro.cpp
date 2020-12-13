#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleCentralEditor.h"
#include "ModuleCamera3D.h"
#include "Console.h"
#include "GameObject.h"
#include "External Libraries/MathGeoLib/include/Math/Quat.h"
#include "External Libraries/MathGeoLib/include/Geometry/LineSegment.h"
#include "External Libraries/MathGeoLib/include/Geometry/Triangle.h"
#include "External Libraries/SDL/include/SDL_scancode.h"
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
#include "ResourceMesh.h"
#include "ModuleResources.h"
#include "ResourceModel.h"
#include "ModuleInput.h"




ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled, "moduleSceneIntro")
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
	root->SetUUID();
	id_gameobject++;
	grid = CreateGrid();
	camera = (ComponentCamera*)CreateGameObject("Camera", root)->CreateComponent(Component_Type::Camera);
	App->renderer3D->camera_culling = camera;
	id_gameobject++;
	game_object_selected = nullptr;
	int num = 0;

	App->central_editor->LoadScene("Library/Scenes/streetview_intro.scenefalcas");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	delete grid;
	delete root;
	game_object_selected = nullptr;
	DevilCleanUp();

	return true;
}

void ModuleSceneIntro::Draw(GLuint tex)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("Scene");
	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 windowPos = ImGui::GetWindowPos();
	ImVec2 min = ImGui::GetWindowContentRegionMin();
	min.x += windowPos.x;
	min.y += windowPos.y;
	windowSize.y -= min.y-windowPos.y;
	width = windowSize.x;
	height = windowSize.y;
	x = min.x;
	y = min.y;
	ImGui::Image((ImTextureID)tex, windowSize, ImVec2(0, 1), ImVec2(1, 0));
	if (ImGui::IsItemHovered()) {
		mouse_on_scene = true;
	}
	else {
		mouse_on_scene = false;
	}
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("model"))
		{
			IM_ASSERT(payload->DataSize == sizeof(int));
			int payload_id = *(const int*)payload->Data;
			ResourceModel* model = (ResourceModel*)App->resources->RequestResource(payload_id);
			LoadModel(model);
		}
		ImGui::EndDragDropTarget();
	}

	App->central_editor->DrawImGuizmo();
	ImGui::End();
	ImGui::PopStyleVar();

}

bool ModuleSceneIntro::GetDimensionsWindow(float& width, float& height)
{
	if (width == this->width && height == this->height)
		return false;
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
	if (App->input->GetMouseButton(3) == KEY_IDLE) {
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN|| App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			input_letter = Guizmos_Input_Letters::W;
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
			input_letter = Guizmos_Input_Letters::E;
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT)
			input_letter = Guizmos_Input_Letters::R;
	}

	for (int i = 0; i < game_objects_to_delete.size(); i++) {
		GameObject* game_object = SearchGameObject(game_objects_to_delete[i], root);
		game_object->RemoveFromParent();
		delete game_object;
	}
	game_objects_to_delete.clear();
	
	if (App->central_editor->wireframe) {
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
	}
	
	resources_material_to_delete.clear();
	resources_mesh_to_delete.clear();
	resources_model_to_delete.clear();

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
		game_object->SetUUID();
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
		game_object->SetUUID();
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

GameObject* ModuleSceneIntro::SearchGameObject(uint uuid, GameObject* game_object)
{
	if (game_object->GetUUID() == uuid) {
		return game_object;
	}
	for (int i = 0; i < game_object->children.size(); i++) {
		game_object = SearchGameObject(uuid, game_object->children[i]);
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

void ModuleSceneIntro::LoadModel(ResourceModel* model)
{
	std::map<uint, GameObject*> gameobject_final;
	GameObject* parent = CreateGameObject(App->filesystem->GetFileName(model->GetAssetsFile(), true), root);
	gameobject_final[model->GetID()] = parent;
	for (int i = 0; i < model->mesh_keys.size(); i++) {
		int id_mesh = model->mesh_keys[i];
		int id_parent = model->meshes[id_mesh];
		int id_texture= model->textures[id_mesh];
		float4x4 transform= model->transform[id_mesh];
		GameObject* child = CreateGameObject(App->filesystem->GetFileName(model->GetAssetsFile(), true), gameobject_final[id_parent]);
		ComponentTransform* trnas;
		float3 pos, size;
		Quat rot;
		transform.Decompose(pos, rot, size);
		((ComponentTransform*)child->GetComponent(Component_Type::Transform))->SetTransformation(pos, rot, size);
		((ComponentMesh*)child->CreateComponent(Component_Type::Mesh))->ChangeResourceMesh((ResourceMesh*)App->resources->RequestResource(id_mesh));
		if (id_texture != 0) {
			((ComponentMaterial*)child->CreateComponent(Component_Type::Material))->ChangeResourceMaterial((ResourceMaterial*)App->resources->RequestResource(id_texture));
		}
		gameobject_final[id_mesh] = child;
	}
	gameobject_final.clear();
	parent->resource_model = model;
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
		for (int i = 0; i < mesh->resource_mesh->num_indices; i+=3) {
			Triangle tri;
			tri.a = float3(&mesh->resource_mesh->vertices[mesh->resource_mesh->indices[i]*3]);
			tri.b = float3(&mesh->resource_mesh->vertices[mesh->resource_mesh->indices[i+1] * 3]);
			tri.c = float3(&mesh->resource_mesh->vertices[mesh->resource_mesh->indices[i+2] * 3]);
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
	if (game_obj->id >= 0 && game_obj->HasComponentType(Component_Type::Mesh) && ((ComponentMesh*)game_obj->GetComponent(Component_Type::Mesh))->resource_mesh != nullptr) {
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

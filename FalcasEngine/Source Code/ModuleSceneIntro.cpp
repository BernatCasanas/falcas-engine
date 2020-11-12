#pragma once
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleCentralEditor.h"
#include "ModuleCamera3D.h"

#include "Console.h"
#include "GameObject.h"
#include "External Libraries/MathGeoLib/include/Math/Quat.h"
#include "External Libraries/Assimp/Assimp/include/scene.h"
#include "ComponentMesh.h"
#include "External Libraries/Devil/Include/il.h"
#include "ComponentMaterial.h"
#include <gl/GL.h>
#include "Shape.h"
#include "FileSystem.h"

#pragma comment( lib, "Source Code/External Libraries/Devil/lib/ILU.lib" )
#pragma comment( lib, "Source Code/External Libraries/Devil/lib/DevIL.lib" )


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

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	ilEnable(IL_ORIGIN_SET);


	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	id_gameobject = -1;
	root = new GameObject(id_gameobject, "Grid", nullptr, { 0,0,0 }, Quat::identity, { 1,1,1 });
	id_gameobject++;
	ComponentMesh* mesh = (ComponentMesh*)root->CreateComponent(Component_Type::Mesh);
	mesh->grid = true;
	CreateGrid(mesh->num_vertices, mesh->num_indices, mesh->indices, mesh->vertices, { 0,0,0 }, { 500,1,500 });
	mesh->Initialization();
	game_object_selected = nullptr;
	int num = 0;

	std::string path = "Assets/BakerHouse/BakerHouse.fbx";
	const aiScene* scene = mesh->GetSceneOfMeshes(path.c_str());
	aiNode* nod = scene->mRootNode;
	CreateGameObject(nod, scene, path.c_str(), "BakerHouse", root);

	mesh->CleanScene(scene);


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	delete root;
	game_object_selected = nullptr;
	ilDisable(IL_ORIGIN_SET);

	return true;
}

void ModuleSceneIntro::Draw(GLuint tex)
{
	ImGui::Begin("Scene");

	ImVec2 windowSize = ImGui::GetWindowSize();
	ImGui::Image((ImTextureID)tex, windowSize, ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
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


GameObject* ModuleSceneIntro::CreateGameObject(const aiNode* node, const aiScene* scene, std::string file_name, std::string name, GameObject* parent)
{
	GameObject* game_object = nullptr;
	name = CheckNameGameObject(name);
	aiVector3D position, size;
	aiQuaternion rotation;
	node->mTransformation.Decompose(size,rotation, position);
	game_object = new GameObject(App->scene_intro->id_gameobject, name, parent, { position.x,position.y,position.z }, { rotation.x,rotation.y,rotation.z,rotation.w }, { size.x,size.y,size.z });
	if (game_object != nullptr) {
		App->scene_intro->id_gameobject++;
		if (parent != nullptr) {
			parent->children.push_back(game_object);
		}
	}
	if (node->mNumMeshes > 0) {
		ComponentMesh* mesh = (ComponentMesh*)game_object->CreateComponent(Component_Type::Mesh);
		int num_material = mesh->LoadMesh(node->mMeshes[0], scene);
		mesh->SetFileName(file_name);
		if (num_material != -1&& num_material<scene->mNumMaterials) {
			ComponentMaterial* mat = (ComponentMaterial*)game_object->CreateComponent(Component_Type::Material);
			aiString material_path;
			scene->mMaterials[num_material]->GetTexture(aiTextureType_DIFFUSE, 0, &material_path);
			if (material_path.length > 0) {
				std::string path = file_name;
				int pos_path = path.find_last_of('/');
				if (pos_path == -1)
					pos_path = path.find_last_of('\\');
				path = path.substr(0, pos_path + 1);
				path += material_path.C_Str();
				mat->LoadTexture(path.c_str());
			}
		}
	}
	for (int i = 0; i < node->mNumChildren; i++) {
		CreateGameObject(node->mChildren[i], scene, file_name, name, game_object);
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
		game_object = SearchGameObject(id, game_obj->children.at(i));
	}
	return game_object;
}

std::string ModuleSceneIntro::CheckNameGameObject(std::string name, bool numbered, int digits)
{
	bool stop = false;
	GameObject* game_object = nullptr;
	for (int i = 0; i <= id_gameobject && stop == false; i++) {
		game_object = SearchGameObject(i, App->scene_intro->root);
		if (game_object != nullptr && game_object->name == name) {
			stop = true;
		}
	}
	if (stop == true) {
		if (numbered) {
			name.pop_back();
			int number = name.back() - '0';
			if (number >= 9)
				name = NameGameObjectWhenMoreThan2Digits(name, digits);
			else {
				number++;
				name.pop_back();
				name.push_back(number + '0');
				name.push_back(')');
			}
		}
		else {
			name.push_back(' ');
			name.push_back('(');
			name.push_back('1');
			name.push_back(')');
		}
		name = CheckNameGameObject(name, true, digits);
	}
	return name;
}

std::string ModuleSceneIntro::NameGameObjectWhenMoreThan2Digits(std::string name, int& digits)
{
	int number = 0;
	for (int i = 0; i < digits; i++) {
		number += (name.back() - '0') * (pow(10, i));
		name.pop_back();
	}
	number++;
	if (pow(10, digits) <= number) {
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
	return name;
}
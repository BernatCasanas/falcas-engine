#pragma once
#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleCentralEditor.h"
#include "ModuleCamera3D.h"
#include <gl/GL.h>
#include "External Libraries/Assimp/Assimp/include/cimport.h"
#include "External Libraries/Assimp/Assimp/include/postprocess.h"
#include "External Libraries/Assimp/Assimp/include/scene.h"
#include "Console.h"
#include "Mesh.h"
#include "GameObject.h"
#include "External Libraries/MathGeoLib/include/Math/Quat.h"
#include "ComponentMesh.h"
#include "External Libraries/Devil/Include/il.h"
#include <string>
#include "ComponentMaterial.h"

#pragma comment( lib, "Source Code/External Libraries/Devil/lib/ILU.lib" )
#pragma comment( lib, "Source Code/External Libraries/Devil/lib/DevIL.lib" )


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	loading = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;


	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	id_gameobject = -1;
	root = new GameObject(id_gameobject, "Grid", nullptr, { 0,0,0 }, Quat::identity, { 500,1,500 });
	id_gameobject++;
	root->CreateComponent(Component_Type::Mesh);
	ComponentMesh* mesh = (ComponentMesh*)root->components.back();
	mesh->CreateGrid({ 0,0,0 }, { 500,1,500 });
	game_object_selected = nullptr;
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	delete root;

	return true;
}

GameObject* ModuleSceneIntro::CreateGameObject(std::string name, GameObject* parent)
{

	GameObject* game_object = nullptr;
	name = CheckNameGameObject(name);
	game_object = new GameObject(id_gameobject, name, parent);
	if (game_object != nullptr) {
		id_gameobject++;
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
	game_object = new GameObject(id_gameobject, name, parent, position, rotation, size);
	if (game_object != nullptr) {
		id_gameobject++;
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
		game_object = SearchGameObject(id, game_obj->children.at(i));
	}
	return game_object;
}


void ModuleSceneIntro::LoadGameObject(float3 position, const char* file, std::string name)
{
	loading = true;
	GameObject* parent = nullptr;
	GameObject* m;
	bool multimesh = false;
	bool parent_setted = false;
	const aiScene* scene = nullptr;
	scene = aiImportFile(file, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		if (scene->mNumMeshes > 1) {
			multimesh = true;
		}
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			if (scene->mMeshes[i] == NULL) continue;
			if (multimesh) {
				if (parent_setted) {
					m = CreateGameObject(position, Quat::identity, { 1,1,1 }, name.c_str(), parent);
				}
				else {
					parent = CreateGameObject(position, Quat::identity, { 1,1,1 }, name.c_str(), root);
					m = CreateGameObject(position, Quat::identity, { 1,1,1 }, name.c_str(), parent);
					parent_setted = true;
				}
			}
			else{
				m = CreateGameObject(position, Quat::identity, { 1,1,1 }, name.c_str(), root);
			}
			ComponentMesh* m_mesh = (ComponentMesh*)m->CreateComponent(Component_Type::Mesh);
			m_mesh->SetFileName(file);
			aiMesh* ai_mesh = scene->mMeshes[i];
			m_mesh->num_vertices = ai_mesh->mNumVertices;
			m_mesh->vertices = new float[m_mesh->num_vertices * 3];
			memcpy(m_mesh->vertices, ai_mesh->mVertices, sizeof(float) * m_mesh->num_vertices);
			LOG("Loading FBX correctly");
			LOG("New mesh with %d vertices", m_mesh->num_vertices);

			if (ai_mesh->HasFaces())
			{
				m_mesh->num_indices = ai_mesh->mNumFaces * 3;
				m_mesh->indices = new uint[m_mesh->num_indices];
				for (uint j = 0; j < ai_mesh->mNumFaces; ++j)
				{
					if (ai_mesh->mFaces[j].mNumIndices != 3) {
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else {
						memcpy(&m_mesh->indices[j * 3], ai_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
					}

				}
				LOG("New mesh with %d index", m_mesh->num_indices);
			}
			m_mesh->num_normals = m_mesh->num_vertices;
			m_mesh->normals = new float[ai_mesh->mNumVertices * 3];
			for (int x = 0, y = 0; x < ai_mesh->mNumVertices; x++, y += 3) {
				if (ai_mesh->HasNormals())
				{
					//normal copying
					m_mesh->normals[y] = ai_mesh->mNormals[x].x;
					m_mesh->normals[y + 1] = ai_mesh->mNormals[x].y;
					m_mesh->normals[y + 2] = ai_mesh->mNormals[x].z;
				}
			}

			if (ai_mesh->HasTextureCoords(0)) {
				m_mesh->num_textureCoords = ai_mesh->mNumVertices;
				m_mesh->texCoords = new float[m_mesh->num_textureCoords * 2];
				for (uint i = 0, j = 0; i < m_mesh->num_textureCoords; i++, j += 2) {
					m_mesh->texCoords[j] = ai_mesh->mTextureCoords[0][i].x;
					m_mesh->texCoords[j + 1] = ai_mesh->mTextureCoords[0][i].y;
				}
			}

			m_mesh->Initialization();
		}
			aiReleaseImport(scene);
	}
	else {
		const char* error = aiGetErrorString();
		LOG("Error loading FBX: %s", error)
	}
	loading = false;
}

std::string ModuleSceneIntro::CheckNameGameObject(std::string name, bool numbered, int digits)
{
	bool stop = false;
	GameObject* game_object = nullptr;
	for (int i = 0; i <= id_gameobject && stop == false; i++) {
		game_object = SearchGameObject(i, root);
		if (game_object != nullptr && game_object->name == name) {
			stop = true;
		}
	}
	if (stop == true) {
		if (numbered){
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

std::string ModuleSceneIntro::NameGameObjectWhenMoreThan2Digits(std::string name, int &digits)
{
	int number = 0;
	for (int i = 0; i < digits; i++) {
		number += (name.back() - '0') * (pow(10, i));
		name.pop_back();
	}
	number++;
	if(pow(10,digits)<=number){
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

int ModuleSceneIntro::GetID()
{
	return id_gameobject;
}


// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	ComponentMesh* mesh = (ComponentMesh*)root->GetComponent(Component_Type::Mesh);
	if (mesh != nullptr && mesh->active == true) {
		mesh->Render();
	}
	GameObject* game_object = nullptr;
	mesh = nullptr;
	for (int i = 0; i < id_gameobject; i++) {
		game_object = SearchGameObject(i, root);
		if (game_object != nullptr && game_object->active == true&& game_object->CheckComponentType(Component_Type::Mesh)) {
			mesh = (ComponentMesh*)game_object->GetComponent(Component_Type::Mesh);
			if (mesh != nullptr && mesh->active == true) {
				mesh->Render();
			}
			mesh = nullptr;
		}
	}
	
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


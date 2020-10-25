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

	total_game_objects =  0;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	id_gameobject = 0;
	grid = new GameObject(id_gameobject, "Grid", nullptr, { 0,0,0 }, Quat::identity, { 500,1,500 });
	grid->CreateComponent(Component_Type::Mesh);
	ComponentMesh* mesh = (ComponentMesh*)grid->components.back();
	mesh->CreateGrid({ 0,0,0 }, { 500,1,500 });
	game_object_selected = nullptr;
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	for (int i = 0; i < total_game_objects; i++) {
		delete game_objects.at(i);
	}
	game_objects.clear();
	delete grid;

	return true;
}

GameObject* ModuleSceneIntro::CreateGameObject(std::string name, GameObject* parent)
{

	GameObject* game_object = nullptr;
	name = CheckNameGameObject(name);
	game_object = new GameObject(id_gameobject, name, parent);
	if (game_object != nullptr) {
		id_gameobject++;
		game_objects.push_back(game_object);
		total_game_objects++;
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
		game_objects.push_back(game_object);
		total_game_objects++;
	}
	return game_object;
}

void ModuleSceneIntro::LoadGameObject(float3 position, char* file, char* name)
{
	scene = aiImportFile(file, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			GameObject* m = CreateGameObject(position, Quat::identity, { 1,1,1 }, name, nullptr);
			ComponentMesh* m_mesh = (ComponentMesh*)m->CreateComponent(Component_Type::Mesh);
			aiMesh* ai_mesh = scene->mMeshes[i];
			m_mesh->num_vertices = ai_mesh->mNumVertices * 3;
			m_mesh->vertices = new float[m_mesh->num_vertices];
			memcpy(m_mesh->vertices, ai_mesh->mVertices, sizeof(float) * m_mesh->num_vertices);
			LOG("Loading FBX correctly");
			LOG("New mesh with %d vertices", m_mesh->num_vertices);



			if (ai_mesh->HasFaces())
			{
				m_mesh->num_indices = ai_mesh->mNumFaces * 3;
				m_mesh->indices = new uint[m_mesh->num_indices]; // assume each face is a triangle
				for (uint j = 0; j < ai_mesh->mNumFaces; ++j)
				{
					if (ai_mesh->mFaces[j].mNumIndices != 3) {
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else {
						memcpy(&m_mesh->indices[j * 3], ai_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
					}

				}
			}

			m_mesh->Initialization();
			aiReleaseImport(scene);
		}
	}
	else {
		const char* error = aiGetErrorString();
		LOG("Error loading FBX: %s", error)
	}
}

std::string ModuleSceneIntro::CheckNameGameObject(std::string name, bool numbered)
{
	bool stop = false;
	for (int i = 0; i < game_objects.size() && stop == false; i++) {
		if (game_objects.at(i)->name == name) {
			stop = true;
		}
	}
	if (stop == true) {
		if (numbered){
			int number = name.back() - '0';
			number++;
			name.pop_back();
			name.push_back(number + '0');
		}
		else {
			name.push_back('1');
		}
		name = CheckNameGameObject(name, true);
	}
	return name;
}


// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	ComponentMesh* mesh = (ComponentMesh*)grid->GetComponent(Component_Type::Mesh);
	if (mesh != nullptr && mesh->active == true) {
		mesh->Render();
	}
	mesh = nullptr;
	for (int i = 0; i < total_game_objects; i++) {
		if (game_objects.at(i)->active == true&&game_objects.at(i)->CheckComponentType(Component_Type::Mesh)) {
			mesh = (ComponentMesh*)game_objects.at(i)->GetComponent(Component_Type::Mesh);
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


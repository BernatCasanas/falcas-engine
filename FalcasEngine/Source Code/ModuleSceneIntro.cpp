#pragma once
#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleCentralEditor.h"
#include "ModuleCamera3D.h"

#include "Console.h"
#include "Mesh.h"
#include "GameObject.h"
#include "External Libraries/MathGeoLib/include/Math/Quat.h"
#include "ComponentMesh.h"
#include "External Libraries/Devil/Include/il.h"
#include <string>
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

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	id_gameobject = -1;
	root = new GameObject(id_gameobject, "Grid", nullptr, { 0,0,0 }, Quat::identity, { 500,1,500 });
	id_gameobject++;
	root->CreateComponent(Component_Type::Mesh);
	ComponentMesh* mesh = (ComponentMesh*)root->components.back();
	mesh->grid = true;
	CreateGrid(mesh->num_vertices, mesh->num_indices, mesh->indices, mesh->vertices, { 0,0,0 }, { 500,1,500 });
	mesh->Initialization();
	game_object_selected = nullptr;

	App->filesystem->CreateCompleteGameObject("BakerHouse", root, "Assets/BakerHouse.fbx", "Assets/Baker_house.png");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

int ModuleSceneIntro::GetID()
{
	return id_gameobject;
}


// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	ComponentMesh* mesh = (ComponentMesh*)root->GetComponent(Component_Type::Mesh);
	if (mesh != nullptr && root->active == true) {
		mesh->Render();
	}
	GameObject* game_object = nullptr;
	mesh = nullptr;
	for (int i = 0; i < id_gameobject; i++) {
		game_object = App->filesystem->SearchGameObject(i, root);
		if (game_object != nullptr && game_object->active == true && game_object->CheckComponentType(Component_Type::Mesh)) {
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


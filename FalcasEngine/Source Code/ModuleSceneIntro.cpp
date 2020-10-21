#pragma once
#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleCentralEditor.h"
#include "ModuleCamera3D.h"
#include <gl/GL.h>
#include "External Libraries/Assimp/Assimp/include/cimport.h"
#include "External Libraries/Assimp/Assimp/include/postprocess.h"
#include "Console.h"
#include "Mesh.h"
#include "Shape.h"



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

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	scene = aiImportFile("Assets/warrior/warrior.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* ai_mesh = scene->mMeshes[i];
			m.num_vertex = ai_mesh->mNumVertices;
			m.vertex = new float[m.num_vertex * 3];
			memcpy(m.vertex, ai_mesh->mVertices, sizeof(float) * m.num_vertex * 3);
			LOG("Loading FBX correctly");
			LOG("New mesh with %d vertices", m.num_vertex);
			
			

			if (ai_mesh->HasFaces())
			{
				m.num_index = ai_mesh->mNumFaces * 3;
				m.index = new uint[m.num_index]; // assume each face is a triangle
				for (uint j = 0; j < ai_mesh->mNumFaces; ++j)
				{
					if (ai_mesh->mFaces[j].mNumIndices != 3) {
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else {
						memcpy(&m.index, ai_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
					}
					
				}
			}
			
			aiReleaseImport(scene);
		}
	}
	else {
		const char* error = aiGetErrorString();
		LOG("Error loading FBX: %s", error)
	}
	grid = new Grid(Shape::Grid, { 0,0,0 }, 500);
	game_object_selected = nullptr;
	total_game_objects = 0;
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

void ModuleSceneIntro::CreateGameObject(Shape shape)
{

	GameObject* game_object = nullptr;
	switch (shape)
	{
	case Shape::Empty:
		game_object = new GameObject(Shape::Empty, { 0,0,0, });
		break;
	case Shape::SolidPlane:
		game_object = new SolidPlane(Shape::SolidPlane, { 0,0,0 }, 1);
		break;
	case Shape::Cube:
		game_object = new Cube(Shape::Cube, { 0,0,0 }, 1);
		break;
	case Shape::RectangularPrism:
		game_object = new RectangularPrism(Shape::RectangularPrism, { 0,0,0 }, 1, 1, 1);
		break;
	case Shape::TriangularPyramid:
		game_object = new TriangularPyramid(Shape::TriangularPyramid, { 0,0,0 }, 1);
		break;
	case Shape::SquarePyramid:
		game_object = new SquarePyramid(Shape::SquarePyramid, { 0,0,0 }, 1, 1);
		break;
	case Shape::RectangularPyramid:
		game_object = new RectangularPyramid(Shape::RectangularPyramid, { 0,0,0 }, 1, 3, 2);
		break;
	case Shape::SolidSphere:
		game_object = new SolidSphere(Shape::SolidSphere, { 0,0,0 }, 1, 12, 24);
		break;
	case Shape::Cilinder:
		game_object = new Cilinder(Shape::Cilinder, { 0,0,0 }, 1, 1, 3, 8);
		break;
	case Shape::SolidCone:
		game_object = new SolidCone(Shape::SolidCone, { 0,0,0 }, 1, 3, 16);
		break;
	}
	if (game_object != nullptr) {
		game_objects.push_back(game_object);
		total_game_objects++;
	}
}

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	grid->Render(false);
	for (int i = 0; i < total_game_objects; i++) {
		game_objects.at(i)->Render();
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


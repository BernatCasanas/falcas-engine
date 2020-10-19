#pragma once
#include "Module.h"
#include "Globals.h"
#include "External Libraries/Assimp/Assimp/include/scene.h"
#include "Mesh.h"


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	const aiScene* scene;


	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	Mesh m;

public:
};

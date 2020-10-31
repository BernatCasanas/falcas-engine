#pragma once

#ifndef _FILE_SYSTEM_
#define _FILE_SYSTEM_

#include "GameObject.h"
#include "Component.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ModuleSceneIntro.h"
#include "Application.h"



class FileSystem : public Module {
public:
	FileSystem(Application* app, bool start_enabled = true);
	~FileSystem();

	bool Start() {};
	update_status Update(float dt) {};
	bool CleanUp() {};

	GameObject* CreateGameObject(std::string name = "", GameObject* parent = nullptr);
	GameObject* CreateGameObject(float3 position, Quat rotation, float3 size, std::string name = "", GameObject* parent = nullptr);
	GameObject* SearchGameObject(int id, GameObject* game_obj);
	std::string CheckNameGameObject(std::string name, bool numbered = false, int digits = 1);
	std::string NameGameObjectWhenMoreThan2Digits(std::string name, int& digits);

public:
	void CreateWhiteGameObject();
	void CreateCompleteGameObject(std::string name, GameObject* parent, std::string mesh_path, std::string material_path = "");

};


#endif // !_FILE_SYSTEM_

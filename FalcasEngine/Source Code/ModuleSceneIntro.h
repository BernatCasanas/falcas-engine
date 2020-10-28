#pragma once
#include "Module.h"
#include "External Libraries/MathGeoLib/include/Math/float3.h"
#include <string>



class GameObject;
struct aiScene;
class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();



	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	

	GameObject* CreateGameObject(std::string name = "", GameObject* parent = nullptr);
	GameObject* CreateGameObject(float3 position, Quat rotation, float3 size, std::string name = "", GameObject* parent = nullptr);
	GameObject* SearchGameObject(int id, GameObject* game_obj);

	void LoadTexture(std::string path);

	void LoadGameObject(float3 position, const char* file, char* name);
	std::string CheckNameGameObject(std::string name, bool numbered = false, int digits = 1);
	std::string NameGameObjectWhenMoreThan2Digits(std::string name, int &digits);

	GameObject* game_object_selected;
	GameObject* root;
	int GetID();
	bool loading;
private:
	int id_gameobject;

public:
};

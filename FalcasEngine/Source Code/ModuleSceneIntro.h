#pragma once
#include "Module.h"
#include "External Libraries/MathGeoLib/include/Math/float3.h"



class GameObject;
struct aiScene;
class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	const aiScene* scene;


	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	

	GameObject* CreateGameObject(std::string name = "", GameObject* parent = nullptr);
	GameObject* CreateGameObject(float3 position, Quat rotation, float3 size, std::string name = "", GameObject* parent = nullptr);

	void LoadGameObject(float3 position, char* file, char* name);
	std::string CheckNameGameObject(std::string name, bool numbered = false);

	GameObject* game_object_selected;
	std::vector<GameObject*> game_objects;
	uint total_game_objects;
private:
	GameObject* grid;
	int id_gameobject;

public:
};

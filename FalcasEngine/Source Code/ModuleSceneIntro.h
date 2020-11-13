#pragma once
#include "Module.h"
#include "External Libraries/MathGeoLib/include/Math/float3.h"
#include <string>
#include "External Libraries/Glew/include/glew.h"


class GameObject;
class ComponentCamera;
class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();



	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Draw(GLuint tex);
	
	GameObject* CreateGameObject(std::string name = "", GameObject* parent = nullptr);
	GameObject* CreateGameObject(float3 position, Quat rotation, float3 size, std::string name = "", GameObject* parent = nullptr);


	GameObject* SearchGameObject(int id, GameObject* game_obj);
	bool IsGameObjectNameRepeated(std::string name, GameObject* game_obj);
	std::string CheckNameGameObject(std::string name, bool numbered = false);

	GameObject* game_object_selected;
	ComponentCamera* camera;
	GameObject* root;
	int GetID();
	int id_gameobject;


private:


};

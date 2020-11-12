#pragma once
#include "Module.h"
#include "External Libraries/MathGeoLib/include/Math/float3.h"
#include <string>
#include "External Libraries/Glew/include/glew.h"

#pragma comment(lib, "Source Code/External Libraries/Glew/libx86/glew32.lib")

class GameObject;
struct aiScene;
struct aiNode;
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
	GameObject* CreateGameObject(const aiNode* node, const aiScene* scene, std::string file_name = "", std::string name = "", GameObject* parent = nullptr);


	GameObject* SearchGameObject(int id, GameObject* game_obj);
	std::string CheckNameGameObject(std::string name, bool numbered = false, int digits = 1);
	std::string NameGameObjectWhenMoreThan2Digits(std::string name, int& digits);

	GameObject* game_object_selected;
	GameObject* root;
	int GetID();
	int id_gameobject;


private:


};

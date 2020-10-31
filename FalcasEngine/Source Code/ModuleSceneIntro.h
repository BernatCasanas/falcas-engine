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
	

	GameObject* game_object_selected;
	GameObject* root;
	int GetID();
	int id_gameobject;

private:


};

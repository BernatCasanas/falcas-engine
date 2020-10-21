#pragma once
#include "Module.h"
#include "Globals.h"
#include "External Libraries/Assimp/Assimp/include/scene.h"
#include "Mesh.h"
#include <vector>


class Grid;
class GameObject;
enum class Shape;
class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	const aiScene* scene;


	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void CreateGameObject(Shape shape);

	Mesh m;
	GameObject* game_object_selected;
	std::vector<GameObject*> game_objects;
	uint total_game_objects;
private:
	Grid* grid;
	uint total_empty;
	uint total_plane;
	uint total_cube;
	uint total_prism;
	uint total_tri_pyr;
	uint total_sqr_pyr;
	uint total_rect_pyr;
	uint total_sphere;
	uint total_cilinder;
	uint total_cone;

public:
};

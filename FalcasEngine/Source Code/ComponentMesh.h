#pragma once
#include "Component.h"
#include "External Libraries/MathGeoLib/include/Geometry/AABB.h"

class GameObject;
class ComponentMesh : public Component {
public:
	ComponentMesh(GameObject* owner);
	~ComponentMesh();
	void Update();
	
	void Initialization();
	void Render();
	bool SaveComponent(JsonObj& obj);

	void Inspector();

	AABB GetAABB()const;


	uint id_vertices = 0;
	uint id_indices = 0;
	uint id_normals = 0;
	uint id_texCoords = 0;
	uint num_indices = 0;
	uint num_vertices = 0;
	uint num_normals = 0;
	uint num_textureCoords = 0;

	std::string full_file_name = "";
	std::string file_name = "";
	float* texCoords = nullptr;
	float* vertices = nullptr;
	float* normals = nullptr;
	uint* indices = nullptr;
	bool show_normals_v = false;
	bool show_normals_f = false;
	float length_normals = 1;
	bool grid = false;
	int materialIndex = 0;
	int  meshNumber = 0;

	std::string libraryPath;

private:
	AABB aabb;
	
	void SetAABB();
};
#pragma once
#include "Component.h"
#include "External Libraries/MathGeoLib/include/Math/float3.h"
#include <vector>

class GameObject;


class ComponentMesh : public Component {
public:
	ComponentMesh(GameObject* owner);
	~ComponentMesh();
	void Update() {};
	void SetFileName(std::string file);
	
	void Initialization();
	void Render();

	void Inspector();

	void LoadMesh(float3 position, const char* file, std::string name);

	uint id_vertices;
	uint id_indices;
	uint id_normals;
	uint id_texCoords;
	uint num_indices;
	uint num_vertices;
	uint num_normals;
	uint num_textureCoords;

	std::string full_file_name;
	std::string file_name;
	float* texCoords;
	float* vertices = nullptr;
	float* normals = nullptr;
	uint* indices = nullptr;
	bool show_normals;
	float length_normals;
	GameObject* parent;
	bool grid;

private:
	bool loading;
};
#pragma once
#include "Component.h"

class GameObject;
struct aiScene;
class ComponentMesh : public Component {
public:
	ComponentMesh(GameObject* owner);
	ComponentMesh(GameObject* owner, char* file);
	~ComponentMesh();
	void Update();
	void SetFileName(std::string file);

	const aiScene* GetNumberOfMeshes(const char* file, int& num);
	
	void LoadMesh(const aiScene* scene, int num_of_mesh = 0);
	void CleanScene(const aiScene* scene);

	void Initialization();
	void Render();

	void Inspector();


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
	bool show_normals_v;
	bool show_normals_f;
	float length_normals;
	bool grid;

private:
};
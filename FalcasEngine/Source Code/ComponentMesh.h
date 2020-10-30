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
	void CreateCube(float3 position = { 0,0,0 }, float3 size = { 1,1,1 });
	void CreatePrism(float3 position = { 0,0,0 }, float3 size = { 1,1,1 });
	void CreateTriPyramid(float3 position = { 0,0,0 }, float3 size = { 1,1,1 });
	void CreateSqrPyramid(float3 position = { 0,0,0 }, float3 size = { 1,1,1 });
	void CreateRectPyramid(float3 position = { 0,0,0 }, float3 size = { 1,1,1 });
	void CreateSphere(float3 position = { 0,0,0 }, float3 size = { 1,1,1 }, uint rings = 24, uint sectors = 24);
	void CreateCilinder(float3 position = { 0,0,0 }, float3 size = { 1,1,1 }, uint rings = 12, uint sectors = 12);
	void CreateCone(float3 position = { 0,0,0 }, float3 size = { 1,1,1 }, uint sectors = 12);
	void CreatePlane(float3 position = { 0,0,0 }, float3 size = { 1,1,1 });
	void CreateGrid(float3 position = { 0,0,0 }, float3 size = { 1,1,1 });
	void Initialization();
	void Render();

	void Inspector();

	void LoadMesh(float3 position, const char* file, std::string name, ComponentMaterial* mat);

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

private:
	bool grid;
	bool loading;
	ComponentMaterial* material;
};
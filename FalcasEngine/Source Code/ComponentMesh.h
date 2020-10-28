#pragma once
#include "Component.h"
#include "External Libraries/MathGeoLib/include/Math/float3.h"


class GameObject;


class ComponentMesh : public Component {
public:
	ComponentMesh(GameObject* owner);
	~ComponentMesh();
	void Update() {};
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

	bool Inspector(Gui_Type& type, int& index, std::string& info, bool*& checked, float& number, bool& same_line, std::string& info2_for_tooltip);


	uint id_vertices;
	uint id_indices;
	uint id_normals;
	uint num_indices;
	uint num_vertices;
	uint num_normals;
	float* vertices = nullptr;
	float* normals = nullptr;
	uint* indices = nullptr;
private:
	bool grid;
};
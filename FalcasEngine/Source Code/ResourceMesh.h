#pragma once
#include "Resource.h"
#include "External Libraries/MathGeoLib/include/Geometry/AABB.h"


class ComponentMaterial;
class ResourceMesh :public Resource {
public:
	ResourceMesh(uint ID, Resource_Type type, std::string assets_file);
	~ResourceMesh() { CleanUp(); };

	bool CleanUp();

	void Initialize();

	void Render(float* transform, ComponentMaterial* mat, bool show_normals_v, float length_normals, bool show_normals_f);

	AABB GetAABB()const;


public:
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

private:
	AABB aabb;

	void SetAABB();

};
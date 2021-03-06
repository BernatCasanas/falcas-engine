#pragma once
#include "Resource.h"
#include <vector>
#include <string>
#include <map>
#include "External Libraries/MathGeoLib/include/Math/float4x4.h"


class ResourceModel :public Resource {
public:
	ResourceModel(uint ID, Resource_Type type, std::string assets_file);
	~ResourceModel() { CleanUp(); };

	bool CleanUp();

public:
	std::map<uint, uint> textures;			//mesh, texture
	std::map<uint, float4x4> transform;		//mesh, transform
	std::map<uint, uint> meshes;			//mesh, parent
	std::vector<uint> mesh_keys;			//mesh
};
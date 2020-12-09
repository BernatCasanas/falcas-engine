#pragma once
#include "Resource.h"
#include <vector>
#include <string>
#include "External Libraries/MathGeoLib/include/MathGeoLib.h"

struct ModelInfo {
	uint UUID = 0;
	uint ParentUUID = 0;
	float4x4 transform = float4x4::identity;
	std::string name = "";
};

class ResourceModel :public Resource {
public:
	ResourceModel(uint ID, Resource_Type type, std::string assets_file);
	~ResourceModel();

public:
	std::vector<uint> meshes;
	std::vector<uint> textures;
	std::vector<ModelInfo> nodes;
};
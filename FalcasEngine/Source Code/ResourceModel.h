#pragma once
#include "Resource.h"
#include <vector>

class ResourceModel :public Resource {
public:
	ResourceModel(uint ID, Resource_Type type, std::string assets_file);
	~ResourceModel();

public:
	std::vector<uint> ids_resources_meshes;
};
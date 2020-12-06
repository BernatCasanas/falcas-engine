#pragma once
#include "Resource.h"

class ResourceMesh :public Resource {
public:
	ResourceMesh(uint ID, Resource_Type type, std::string assets_file);
	~ResourceMesh() {};

public:
};
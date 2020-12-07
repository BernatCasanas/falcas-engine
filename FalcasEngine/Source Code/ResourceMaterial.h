#pragma once
#include "Resource.h"

class ResourceMaterial :public Resource {
public:
	ResourceMaterial(uint ID, Resource_Type type, std::string assets_file);
	~ResourceMaterial() {};
	bool CleanUp();

public:
	uint image_name = 0;
	uint texture_id = 0;
	std::string full_file_name = "";
	std::string file_name = "";

	int width = 0;
	int height = 0;

	uint size = 0;
};
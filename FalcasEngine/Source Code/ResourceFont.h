#pragma once
#include "Resource.h"

class ResourceFont :public Resource {
public:
	ResourceFont(uint ID, Resource_Type type, std::string assets_file);
	~ResourceFont() { CleanUp(); };
	bool CleanUp();

public:
	std::string font_path;

};
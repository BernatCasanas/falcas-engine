#ifndef _COMPONENT_MATERIAL_
#define _COMPONENT_MATERIAL_
#pragma once

#include "Component.h"


class GameObject;

class ComponentMaterial : public Component {
public:
	ComponentMaterial(GameObject* owner);
	ComponentMaterial(GameObject* owner, char* file);
	~ComponentMaterial();
public:
	bool SaveComponent(JsonObj& obj);
	void Inspector();

	
public:
	uint image_name = 0;
	uint checkers = 0;
	uint texture_id = 0;
	uint defaultTex = 0;
	std::string full_file_name = "";
	std::string file_name = "";

	int width = 0;
	int height = 0;

	uint size = 0;

	bool show_default_tex = true;


};
#endif // !_COMPONENT_MATERIAL_
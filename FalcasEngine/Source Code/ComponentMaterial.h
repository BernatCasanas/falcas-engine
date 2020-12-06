#ifndef _COMPONENT_MATERIAL_
#define _COMPONENT_MATERIAL_
#pragma once

#include "Component.h"


class GameObject;
class ResourceMaterial;

class ComponentMaterial : public Component {
public:
	ComponentMaterial(GameObject* owner);
	ComponentMaterial(GameObject* owner, char* file);
	~ComponentMaterial();
public:
	bool SaveComponent(JsonObj& obj);
	void Inspector();

	
public:
	ResourceMaterial* resource_material = nullptr;
	uint defaultTex = 0;
	bool show_default_tex = false;


};
#endif // !_COMPONENT_MATERIAL_
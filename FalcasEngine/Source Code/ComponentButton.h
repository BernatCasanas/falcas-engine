#pragma once
#include "ComponentUI.h"

class ResourceMaterial;
class ComponentTransform2D;

enum class CLICK {
	ChangeScreen
};

class ComponentButton : public ComponentUI {
public:
	ComponentButton(GameObject* owner, ComponentTransform2D* trans);
	~ComponentButton();
	void Update();

	void Initialization();
	void Render();
	bool SaveComponent(JsonObj& obj);

	void ChangeResourceMaterial(ResourceMaterial* resource_mat, int num_sprite);

	void Inspector();


private:
	ResourceMaterial* resource_material_sprite1 = nullptr;
	ResourceMaterial* resource_material_sprite2 = nullptr;
	ResourceMaterial* resource_material_sprite3 = nullptr;
	ComponentTransform2D* trans;
	std::string scene_name;
	std::string combo = "";
	CLICK functionality;
};

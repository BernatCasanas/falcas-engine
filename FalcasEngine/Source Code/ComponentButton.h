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

	CLICK GetFunctionality();
	std::string GetSceneName();

	void ChangeResourceMaterial(ResourceMaterial* resource_mat, int num_sprite);

	void Inspector();

	void SetMaterialsLoading(ResourceMaterial* _1, ResourceMaterial* _2, ResourceMaterial* _3);
	void SetTrans(ComponentTransform2D* trans);
	void SetSceneName(std::string scene_name);
	void SetFunctionality(std::string functionality);

private:
	ResourceMaterial* resource_material_sprite1 = nullptr;
	ResourceMaterial* resource_material_sprite2 = nullptr;
	ResourceMaterial* resource_material_sprite3 = nullptr;
	ComponentTransform2D* trans;
	std::string scene_name;
	std::string combo = "";
	CLICK functionality;

};

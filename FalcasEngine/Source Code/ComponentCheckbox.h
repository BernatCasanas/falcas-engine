#pragma once
#include "ComponentUI.h"

class ResourceMaterial;
class ComponentTransform2D;
class ComponentCheckbox : public ComponentUI {
public:
	ComponentCheckbox(GameObject* owner, ComponentTransform2D* trans);
	~ComponentCheckbox();
	void Update();

	void Initialization();
	void Render();
	bool SaveComponent(JsonObj& obj);

	void ChangeResourceMaterial(ResourceMaterial* resource_mat, int num_sprite);

	bool IsActive()const { return is_active; }

	void SetMaterialsLoading(ResourceMaterial* _1, ResourceMaterial* _2, ResourceMaterial* _3, ResourceMaterial* _4);
	void SetTrans(ComponentTransform2D* trans);
	void SetActivity(bool active);

	void Inspector();

private:
	ResourceMaterial* resource_material_sprite1 = nullptr;
	ResourceMaterial* resource_material_sprite2 = nullptr;
	ResourceMaterial* resource_material_sprite3 = nullptr;
	ResourceMaterial* resource_material_sprite4 = nullptr;
	ComponentTransform2D* trans;
	bool is_active = false;
};

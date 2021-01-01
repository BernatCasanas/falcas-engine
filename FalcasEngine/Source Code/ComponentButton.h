#pragma once
#include "Component.h"

class ResourceMaterial;
class ResourceMesh;
class ComponentTransform2D;

class ComponentButton : public Component {
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
	ResourceMesh* resource_mesh = nullptr;
	ComponentTransform2D* trans;
	int id_texCoords;

};

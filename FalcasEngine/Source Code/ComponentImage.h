#pragma once
#include "Component.h"

class ResourceMaterial;
class ResourceMesh;
class ComponentTransform2D;

class ComponentImage : public Component {
public:
	ComponentImage(GameObject* owner, ComponentTransform2D* trans);
	~ComponentImage();
	void Update();

	void Initialization();
	void Render();
	bool SaveComponent(JsonObj& obj);

	void ChangeResourceMaterial(ResourceMaterial* resource_mat);

	void Inspector();

public:
	ResourceMaterial* resource_material = nullptr;
	ResourceMesh* resource_mesh = nullptr;
private:
	ComponentTransform2D* trans;
	int id_texCoords;
};

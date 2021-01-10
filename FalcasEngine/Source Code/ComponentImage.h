#pragma once
#include "ComponentUI.h"

class ResourceMaterial;
class ComponentTransform2D;

class ComponentImage : public ComponentUI {
public:
	ComponentImage(GameObject* owner, ComponentTransform2D* trans);
	~ComponentImage();
	void Update();

	void Initialization();
	void Render();
	bool SaveComponent(JsonObj& obj);

	void ChangeResourceMaterial(ResourceMaterial* resource_mat);

	void Inspector();

	void SetMaterialLoading(ResourceMaterial* _1);
	void SetTrans(ComponentTransform2D* trans);

	void RenderImage();

private:
	ResourceMaterial* resource_material = nullptr;
	ComponentTransform2D* trans;
};

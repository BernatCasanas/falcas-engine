#pragma once
#include "ComponentUI.h"
#include "GLFont.h"
#include "FTLabel.h"


class ResourceMaterial;
class ComponentTransform2D;


class ComponentFont :public ComponentUI {
public:
	ComponentFont (GameObject* owner, ComponentTransform2D* trans);
	~ComponentFont();

	void Update();

	void Initialization();
	void Render();
	bool SaveComponent(JsonObj& obj);


	void Inspector();

private:
	std::string text;
	ComponentTransform2D* trans;
	std::shared_ptr<GLFont> font;
	std::unique_ptr<FTLabel> label;
};
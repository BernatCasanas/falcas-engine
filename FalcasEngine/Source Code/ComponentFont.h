#pragma once
#include "ComponentUI.h"
#include "GLFont.h"
#include "FTLabel.h"
#include "External Libraries/ImGui/imgui.h"
#include <map>

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

	void SetTrans(ComponentTransform2D* trans);

private:
	ComponentTransform2D* trans;
	bool init = false;
};
#pragma once
#include "ComponentUI.h"

class ResourceMaterial;
class ComponentTransform2D;

struct Font
{
	char table[256];
	ResourceMaterial* texture = nullptr;
	uint rows, len, char_w, char_h, row_chars;
};		

class ComponentFont :public ComponentUI {
public:
	ComponentFont (GameObject* owner, ComponentTransform2D* trans);
	~ComponentFont();

	void Update();

	void Initialization();
	void Render();
	void RenderChar(ResourceMaterial* c, uint pos);
	bool SaveComponent(JsonObj& obj);

	void Inspector();

	void Load(uint id, std::string characters, uint rows, uint h, uint w, uint rc);


private:
	std::string text;
	Font font;
	ComponentTransform2D* trans;
};
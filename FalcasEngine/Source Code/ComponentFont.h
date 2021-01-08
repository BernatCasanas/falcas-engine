#pragma once
#include "ComponentUI.h"
#include <map>
#include "External Libraries/ImGui/imgui.h"
#include "External Libraries/FreeType/include/freetype/freetype.h"

class ResourceMaterial;
class ComponentTransform2D;

struct Character {
	unsigned int TextureID;  
	ImVec2		 Size;       
	ImVec2		 Bearing;
	unsigned int Advance;    
};


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
	std::map<char, Character> chars;
	ComponentTransform2D* trans;
	FT_Face face;

};
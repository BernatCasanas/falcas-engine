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
	void SetText(std::string text);
	void SetAligment(int alig);
	void SetTypeSize(std::string type, std::string size);
	void SetColor(JsonObj& obj);

private:
	std::string text;
	ComponentTransform2D* trans;
	std::map<std::string,std::shared_ptr<GLFont>> fonts;
	std::unique_ptr<FTLabel> label;
	FTLabel::FontFlags aligment;
	ImVec4 color;
	std::string typeFont = "Arial";
	std::string fontSize = "64";
	bool init = false;
};
#pragma once
#include "ComponentFont.h"
#include "Application.h"
#include "ModuleResources.h"
#include "ResourceMaterial.h"
#include "ModuleUI.h"
#include "ModuleSceneIntro.h"
#include "ResourceMesh.h"
#include "External Libraries/ImGui/imgui.h"
#include "ComponentTransform2D.h"
#include "ModuleWindow.h"
#include <iostream>


ComponentFont::ComponentFont(GameObject* owner, ComponentTransform2D* trans) : ComponentUI(Component_Type::Font, owner, "Text"), trans(trans)
{
	aligment = FTLabel::FontFlags::RightAligned;
}

ComponentFont::~ComponentFont()
{
}

void ComponentFont::Update()
{
	ComponentUI::Update();
	if (init == false) {
		Initialization();
		init = true;
	} 
	if (is_clicked_first_frame) {
		OnTriggered(this);
	}
}

void ComponentFont::Initialization()
{
	fonts["Arial"] = std::shared_ptr<GLFont>(new GLFont("Assets/Fonts/arial.ttf"));
	fonts["Calibri"] = std::shared_ptr<GLFont>(new GLFont("Assets/Fonts/Calibri Regular.ttf"));
	fonts["Roboto"] = std::shared_ptr<GLFont>(new GLFont("Assets/Fonts/Roboto-Regular.ttf"));
	fonts["EastSeaDokdo"] = std::shared_ptr<GLFont>(new GLFont("Assets/Fonts/EastSeaDokdo-Regular.ttf"));


	label = std::unique_ptr<FTLabel>(new FTLabel(
		fonts["Arial"],
		text.c_str(),
		trans->GetPosition().x,
		trans->GetPosition().y,
		App->window->width,
		App->window->height
	));
	label->setColor(0.89, 0.26, 0.3, 0.9);
	label->setPixelSize(64);
	label->setAlignment(aligment);
	label->appendFontFlags(FTLabel::FontFlags::Indented);

}

void ComponentFont::Render()
{
	if (!active)
		return;
	label.get()->setFont(fonts[typeFont]);
	if (fontSize == "") fontSize = "0";
	label.get()->setPixelSize(std::stoi(fontSize));
	label.get()->setText((char*)text.c_str());
	label.get()->setAlignment(aligment);
	label.get()->setPosition(trans->GetPosition().x, trans->GetPosition().y);
	label.get()->setColor(color.x, color.y, color.z, color.w);

	label.get()->render();

}

bool ComponentFont::SaveComponent(JsonObj& obj)
{
	obj.AddString("Text", text.c_str());
	obj.AddFloat4x4("Matrix", trans->GetGlobalMatrix());
	obj.AddInt("Aligment", aligment);
	obj.AddString("TypeFont", typeFont.c_str());
	obj.AddString("FontSize", fontSize.c_str());
	return true;
}

void ComponentFont::Inspector()
{
	ImGui::PushID(name.c_str());
	Component::Inspector();
	ComponentUI::Inspector();
	ImGui::Separator();

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Text: ");
	ImGui::SameLine();
	ImGui::InputText("##text", &text, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll);
	ImGui::SameLine();
	ImGui::InputText("##text1", &fontSize, ImGuiInputTextFlags_EnterReturnsTrue);
	ImGui::Separator();
	ImGui::Text("Font Type: ");
	const char* items[4] = { "Arial","Calibri","Roboto","EastSeaDokdo" };
	if (ImGui::BeginCombo("##combo", typeFont.c_str()))
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			bool is_selected = (typeFont == items[n]);
			if (ImGui::Selectable(items[n], is_selected)) {
				typeFont = items[n];
				if (typeFont == "Arial") {
				}
				if (typeFont == "Calibri") {
				}
				if (typeFont == "Roboto") {
				}
				if (typeFont == "EastSeaDokdo") {
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Separator();

	if (ImGui::Button("Left")) {
		aligment = FTLabel::FontFlags::LeftAligned;
	}
	ImGui::SameLine();
	if (ImGui::Button("Center")) {
		aligment = FTLabel::FontFlags::CenterAligned;
	}
	ImGui::SameLine();
	if (ImGui::Button("Right")) {
		aligment = FTLabel::FontFlags::RightAligned;
	}

	ImGui::Separator();

	ImGui::ColorPicker4("Color Picker", (float*)&color);
	


	ImGui::PopID();
}

void ComponentFont::SetTrans(ComponentTransform2D* trans)
{
	this->trans = trans;
}

void ComponentFont::SetText(std::string text)
{
	this->text = text;
}

void ComponentFont::SetAligment(int alig)
{
	switch (alig)
	{
	case 2:
		aligment = FTLabel::FontFlags::LeftAligned;
		break;
	case 4:
		aligment = FTLabel::FontFlags::CenterAligned;
		break;
	case 8:
		aligment = FTLabel::FontFlags::RightAligned;
		break;
	default:
		break;
	}
}

void ComponentFont::SetTypeSize(std::string type, std::string size)
{
	typeFont = type;
	fontSize = size;
}

void ComponentFont::SetColor(JsonObj& obj)
{
	color = ImVec4(obj.GetInt("r"), obj.GetInt("g"), obj.GetInt("b"), obj.GetInt("a"));
}

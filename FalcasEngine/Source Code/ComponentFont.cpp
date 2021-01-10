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
	_label->setColor(0.89, 0.26, 0.3, 0.9);
	_label->setPixelSize(64);
	_label->setAlignment(aligment);
	_label->appendFontFlags(FTLabel::FontFlags::Indented);

}

void ComponentFont::Render()
{
	if (!active)
		return;
	PrintText(_text, _size, _color, trans);

}

bool ComponentFont::SaveComponent(JsonObj& obj)
{
	SaveGeneralStuff(obj);
	return true;
}

void ComponentFont::Inspector()
{
	ImGui::PushID(name.c_str());
	Component::Inspector();
	ComponentUI::Inspector();
	ComponentUI::TextInspector();
	


	ImGui::PopID();
}

void ComponentFont::SetTrans(ComponentTransform2D* trans)
{
	this->trans = trans;
}
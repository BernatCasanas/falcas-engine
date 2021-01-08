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


ComponentFont::ComponentFont(GameObject* owner, ComponentTransform2D* trans) : ComponentUI(Component_Type::Font, owner, "Text"), trans(trans)
{
	Initialization();
}

ComponentFont::~ComponentFont()
{
}

void ComponentFont::Update()
{
	ComponentUI::Update();
	if (is_clicked_first_frame) {
		OnTriggered(this);
	}
}

void ComponentFont::Initialization()
{
	font = std::shared_ptr<GLFont>(new GLFont("Assets/Fonts/Calibri Regular.ttf"));
	label = std::unique_ptr<FTLabel>(new FTLabel(
		font,
		text.c_str(),
		trans->GetPosition().x,
		trans->GetPosition().y,
		App->window->width,
		App->window->height
	));
	label->setColor(0.89, 0.26, 0.3, 0.9);
	label->setPixelSize(64);
	label->setAlignment(FTLabel::FontFlags::CenterAligned);
	label->appendFontFlags(FTLabel::FontFlags::Indented);
}

void ComponentFont::Render()
{
	if (!active)
		return;
	
	label.get()->render();

}

bool ComponentFont::SaveComponent(JsonObj& obj)
{
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

	ImGui::Separator();


		
	ImGui::PopID();
}

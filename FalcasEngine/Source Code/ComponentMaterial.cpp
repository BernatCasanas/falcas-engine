#include "ComponentMaterial.h"
#include "External Libraries/ImGui/imgui.h"
#include "FileSystem.h"
#include "Application.h"
#include "Importer.h"
#include "GameObject.h"



ComponentMaterial::ComponentMaterial(GameObject* owner) : Component(Component_Type::Material, owner, "Material")
{
	ImportDefaultTexture(this);
}

ComponentMaterial::ComponentMaterial(GameObject* owner, char* file) : Component(Component_Type::Material, owner, "Material")
{
	ImportDefaultTexture(this);
	TextureImporter::Import(this, file, false,"");
}

ComponentMaterial::~ComponentMaterial()
{

}


bool ComponentMaterial::SaveComponent(JsonObj& obj)
{
	obj.AddString("Path", this->file_name.c_str());
	obj.AddInt("UUID", GetUUID());
	return true;
}

void ComponentMaterial::Inspector()
{
	bool falsed = false;

	ImGui::PushID(name.c_str());
	Component::Inspector();

	ImGui::Separator();

	ImGui::AlignTextToFramePadding();
	ImGui::Text("File: ");

	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), file_name.c_str());

	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip(full_file_name.c_str());
	}

	ImGui::Separator();


	ImGui::AlignTextToFramePadding();
	ImGui::Text("Size: ");

	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(std::to_string(width).c_str());

	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(" x ");

	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(std::to_string(height).c_str());


	ImGui::Image((void*)(intptr_t)texture_id, ImVec2((float)128, (float)128), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));


	ImGui::Separator();


	ImGui::Checkbox("Checkers Texture", (active && owner->active) ? &show_default_tex : &falsed);

	ImGui::Separator();

	ImGui::PopID();
}

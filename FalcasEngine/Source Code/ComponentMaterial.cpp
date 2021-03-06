#include "ComponentMaterial.h"
#include "External Libraries/ImGui/imgui.h"
#include "FileSystem.h"
#include "Application.h"
#include "Importer.h"
#include "GameObject.h"
#include "ResourceMaterial.h"
#include "ModuleResources.h"
#include "ModuleSceneIntro.h"




ComponentMaterial::ComponentMaterial(GameObject* owner) : Component(Component_Type::Material, owner, "Material")
{
	ImportDefaultTexture(this);
}

ComponentMaterial::ComponentMaterial(GameObject* owner, char* file) : Component(Component_Type::Material, owner, "Material")
{
	ImportDefaultTexture(this);
}

ComponentMaterial::~ComponentMaterial()
{
	if (resource_material != nullptr && !App->resources->isResourcesMapEmpty()) {
		App->resources->FreeResource(resource_material);
	}
	resource_material = nullptr;
}

void ComponentMaterial::Update()
{
	for (int i = 0; i < App->scene_intro->resources_material_to_delete.size(); i++) {
		if (resource_material == App->scene_intro->resources_material_to_delete[i]) {
			resource_material = nullptr;
			return;
		}
	}
}

void ComponentMaterial::ChangeResourceMaterial(ResourceMaterial* resource_mat)
{
	if (resource_material != nullptr) {
		App->resources->FreeResource(resource_material);
	}
	resource_material = resource_mat;
}


bool ComponentMaterial::SaveComponent(JsonObj& obj)
{
	obj.AddString("Resource_String", resource_material != nullptr ? resource_material->GetAssetsFile() : "");
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
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), resource_material != nullptr ? resource_material->file_name.c_str() : "None");
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("texture"))
		{
			IM_ASSERT(payload->DataSize == sizeof(int));
			int payload_id = *(const int*)payload->Data;
			ChangeResourceMaterial((ResourceMaterial*)App->resources->RequestResource(payload_id));
		}
		ImGui::EndDragDropTarget();
	}

	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip(resource_material != nullptr ? resource_material->full_file_name.c_str() : "");
	}

	if (ImGui::Button("Load Material")) {
		ImGui::OpenPopup("load material");
	}
	if (ImGui::BeginPopupModal("load material")) {
		std::vector<std::string> files;
		std::vector<uint> ids;
		App->filesystem->DiscoverFilesLibrary("Library/Textures/", files, ids);

		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild("Textures", ImVec2(0, 300), true);
		for (int i = 0; i < files.size(); i++) {
			ImGui::Selectable(files[i].c_str());
			if (ImGui::IsItemClicked()) {
				ChangeResourceMaterial((ResourceMaterial*)App->resources->RequestResource(ids[i]));
				ImGui::CloseCurrentPopup();
			}
		}
		ImGui::EndChild();
		ImGui::PopStyleVar();
		if (ImGui::Button("Cancel", ImVec2(50, 20)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::Separator();


	ImGui::AlignTextToFramePadding();
	ImGui::Text("Size: ");

	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(std::to_string(resource_material != nullptr ? resource_material->width : 0).c_str());

	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(" x ");

	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(std::to_string(resource_material != nullptr ? resource_material->height : 0).c_str());

	if(resource_material!=nullptr)
		ImGui::Image((void*)(intptr_t)resource_material->texture_id, ImVec2((float)128, (float)128), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));


	ImGui::Separator();


	ImGui::Checkbox("Checkers Texture", (active && owner->active) ? &show_default_tex : &falsed);

	ImGui::Separator();

	ImGui::PopID();
}

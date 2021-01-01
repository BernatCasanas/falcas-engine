#include "ComponentButton.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleResources.h"
#include "ResourceMesh.h"
#include "ResourceMaterial.h"
#include "ComponentTransform2D.h"
#include "ModuleUI.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "External Libraries/ImGui/imgui.h"



#include "ModuleInput.h"

ComponentButton::ComponentButton(GameObject* owner, ComponentTransform2D* trans) : ComponentUI(Component_Type::Button, owner, "Button"), trans(trans)
{
}


ComponentButton::~ComponentButton()
{
	if (resource_material_sprite1 != nullptr && !App->resources->isResourcesMapEmpty()) {
		App->resources->FreeResource(resource_material_sprite1);
	}
	resource_material_sprite1 = nullptr;
	if (resource_material_sprite2 != nullptr && !App->resources->isResourcesMapEmpty()) {
		App->resources->FreeResource(resource_material_sprite2);
	}
	resource_material_sprite2 = nullptr;
	if (resource_material_sprite3 != nullptr && !App->resources->isResourcesMapEmpty()) {
		App->resources->FreeResource(resource_material_sprite3);
	}
	resource_material_sprite3 = nullptr;
}

void ComponentButton::Update()
{
	for (int i = 0; i < App->scene_intro->resources_material_to_delete.size(); i++) {
		if (resource_material_sprite1 == App->scene_intro->resources_material_to_delete[i]) {
			resource_material_sprite1 = nullptr;
		}
		if (resource_material_sprite2 == App->scene_intro->resources_material_to_delete[i]) {
			resource_material_sprite2 = nullptr;
		}
		if (resource_material_sprite3 == App->scene_intro->resources_material_to_delete[i]) {
			resource_material_sprite3 = nullptr;
		}
	}
	if (App->input->GetKey(11) == KEY_DOWN) {
		GetPlane();
	}
}

void ComponentButton::Initialization()
{
}

void ComponentButton::Render()
{
	if (!active)
		return;
	resource_mesh->Render((float*)&trans->GetGlobalMatrixTransposed(), nullptr, false, false, false, resource_material_sprite1);
}

bool ComponentButton::SaveComponent(JsonObj& obj)
{
	//obj.AddInt("Resource_ID", resource_material != nullptr ? resource_material->GetID() : 0);
	return true;
}

void ComponentButton::ChangeResourceMaterial(ResourceMaterial* resource_mat, int num_sprite)
{
	switch (num_sprite)
	{
	case 1:
		if (resource_material_sprite1 != nullptr) {
			App->resources->FreeResource(resource_material_sprite1);
		}
		resource_material_sprite1 = resource_mat;
		break;
	case 2:
		if (resource_material_sprite2 != nullptr) {
			App->resources->FreeResource(resource_material_sprite2);
		}
		resource_material_sprite2 = resource_mat;
		break;
	case 3:
		if (resource_material_sprite3 != nullptr) {
			App->resources->FreeResource(resource_material_sprite3);
		}
		resource_material_sprite3 = resource_mat;
		break;
	}
	
}

void ComponentButton::Inspector()
{
	ImGui::PushID(name.c_str());
	Component::Inspector();
	ImGui::Separator();

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Sprite 1:");
	ImGui::Separator();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("File: ");

	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), resource_material_sprite1 != nullptr ? resource_material_sprite1->file_name.c_str() : "None");
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("texture"))
		{
			IM_ASSERT(payload->DataSize == sizeof(int));
			int payload_id = *(const int*)payload->Data;
			ChangeResourceMaterial((ResourceMaterial*)App->resources->RequestResource(payload_id), 1);
		}
		ImGui::EndDragDropTarget();
	}

	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip(resource_material_sprite1 != nullptr ? resource_material_sprite1->full_file_name.c_str() : "");
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
				ChangeResourceMaterial((ResourceMaterial*)App->resources->RequestResource(ids[i]), 1);
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
	ImGui::Text(std::to_string(resource_material_sprite1 != nullptr ? resource_material_sprite1->width : 0).c_str());

	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(" x ");

	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(std::to_string(resource_material_sprite1 != nullptr ? resource_material_sprite1->height : 0).c_str());

	if (resource_material_sprite1 != nullptr)
		ImGui::Image((void*)(intptr_t)resource_material_sprite1->texture_id, ImVec2((float)128, (float)128), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));


	ImGui::Separator();

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Sprite 2:");
	ImGui::Separator();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("File: ");

	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), resource_material_sprite2 != nullptr ? resource_material_sprite2->file_name.c_str() : "None");
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("texture"))
		{
			IM_ASSERT(payload->DataSize == sizeof(int));
			int payload_id = *(const int*)payload->Data;
			ChangeResourceMaterial((ResourceMaterial*)App->resources->RequestResource(payload_id), 2);
		}
		ImGui::EndDragDropTarget();
	}

	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip(resource_material_sprite2 != nullptr ? resource_material_sprite2->full_file_name.c_str() : "");
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
				ChangeResourceMaterial((ResourceMaterial*)App->resources->RequestResource(ids[i]), 2);
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
	ImGui::Text(std::to_string(resource_material_sprite2 != nullptr ? resource_material_sprite2->width : 0).c_str());

	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(" x ");

	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(std::to_string(resource_material_sprite2 != nullptr ? resource_material_sprite2->height : 0).c_str());

	if (resource_material_sprite2 != nullptr)
		ImGui::Image((void*)(intptr_t)resource_material_sprite2->texture_id, ImVec2((float)128, (float)128), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));


	ImGui::Separator();

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Sprite 3:");
	ImGui::Separator();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("File: ");

	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), resource_material_sprite3 != nullptr ? resource_material_sprite3->file_name.c_str() : "None");
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("texture"))
		{
			IM_ASSERT(payload->DataSize == sizeof(int));
			int payload_id = *(const int*)payload->Data;
			ChangeResourceMaterial((ResourceMaterial*)App->resources->RequestResource(payload_id), 3);
		}
		ImGui::EndDragDropTarget();
	}

	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip(resource_material_sprite3 != nullptr ? resource_material_sprite3->full_file_name.c_str() : "");
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
				ChangeResourceMaterial((ResourceMaterial*)App->resources->RequestResource(ids[i]), 3);
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
	ImGui::Text(std::to_string(resource_material_sprite3 != nullptr ? resource_material_sprite3->width : 0).c_str());

	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(" x ");

	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(std::to_string(resource_material_sprite3 != nullptr ? resource_material_sprite3->height : 0).c_str());

	if (resource_material_sprite3 != nullptr)
		ImGui::Image((void*)(intptr_t)resource_material_sprite3->texture_id, ImVec2((float)128, (float)128), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));


	ImGui::Separator();

	ImGui::PopID();
}
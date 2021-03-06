#include "ComponentInputbox.h"
#include "Application.h"
#include "ComponentTransform2D.h"
#include "ModuleResources.h"
#include "ModuleSceneIntro.h"
#include "ResourceMesh.h"
#include "ModuleInput.h"
#include "FileSystem.h"
#include "ResourceMaterial.h"
#include "External Libraries/ImGui/imgui.h"
#include "External Libraries/SDL/include/SDL_events.h"


ComponentInputbox::ComponentInputbox(GameObject* owner, ComponentTransform2D* trans) : ComponentUI(Component_Type::Inputbox, owner, "Input Box"), trans(trans)
{
}


ComponentInputbox::~ComponentInputbox()
{
	if(resource_material_sprite != nullptr && !App->resources->isResourcesMapEmpty()) {
		App->resources->FreeResource(resource_material_sprite);
	}
	resource_material_sprite = nullptr;
}

void ComponentInputbox::Update()
{
	ComponentUI::Update();

	if (is_focused == true && text_input_activated == false) {
		SDL_StartTextInput();
		App->input->changed_text_input = false;
		text_input_activated = true;
	}
	else if (is_focused == false && text_input_activated == true) {
		SDL_StopTextInput();
		text_input_activated = false;
	}
	if (is_focused == false)
		return;
	
	if (App->input->changed_text_input) {
		output_timer.Stop();
		App->input->changed_text_input = false;
		if (position == input_text.size()) {
			input_text += App->input->text_input;
		}
		else {
			std::string substring = input_text.substr(0, position);
			input_text = substring + App->input->text_input + input_text.substr(position, input_text.size());
		}
		position++;
	}
	else if (App->input->special_keys == specialkeys::Backspace) {
		output_timer.Stop();
		Backspace(position);
		if (position > 0)
			position--;
	}
	else if (App->input->special_keys == specialkeys::Left) {
		output_timer.Stop();
		if (position > 0)
			position--;
	}
	else if (App->input->special_keys == specialkeys::Right) {
		output_timer.Stop();
		if (position < input_text.size())
			position++;
	}
	else if (App->input->special_keys == specialkeys::Supr) {
		output_timer.Stop();
		Supr(position);
	}
	else if (App->input->special_keys == specialkeys::Home) {
		output_timer.Stop();
		position = 0;
	}
	else if (App->input->special_keys == specialkeys::End) {
		output_timer.Stop();
		position = input_text.size();
	}
	else if (output_timer.isStoped()) {
		output_timer.Start();
	}

	if (is_clicked_with_enter) {
		OnTriggered(this);
	}
	
	if (output_timer.ReadSec() >= 2) {
		output_timer.Start();
	}

	CheckOutputText();
	
	for (int i = 0; i < App->scene_intro->resources_material_to_delete.size(); i++) {
		if (resource_material_sprite == App->scene_intro->resources_material_to_delete[i]) {
			resource_material_sprite = nullptr;
			break;
		}
	}
}

bool ComponentInputbox::SaveComponent(JsonObj& obj)
{
	obj.AddString("Output", output_text.c_str());
	obj.AddString("Input", input_text.c_str());
	obj.AddInt("Pos", position);
	obj.AddBool("Active", text_input_activated);
	obj.AddString("Resource_String", resource_material_sprite != nullptr ? resource_material_sprite->GetAssetsFile() : "");
	SaveGeneralStuff(obj);
	return true;
}

void ComponentInputbox::Initialization()
{
}

void ComponentInputbox::Render()
{
	if (!active)
		return;

	resource_mesh->Render((float*)&trans->GetGlobalMatrixTransposed(), nullptr, false, false, false, resource_material_sprite);

	if (!is_focused && input_text == "") {
		PrintText(_text, _size, _color, trans);
	}
	else {
		PrintText(output_text, _size, _color, trans);
	}

}


void ComponentInputbox::Backspace(int position)
{
	if (position == 0) {
		return;
	}
	if (position == input_text.size()) {
		input_text.pop_back();
	}
	else {
		std::string substring = input_text.substr(0, position - 1);
		input_text = substring + input_text.substr(position, input_text.size());
	}
}

void ComponentInputbox::Supr(int position)
{
	if (position == input_text.size()) {
		return;
	}
	std::string substring = input_text.substr(0, position);
	input_text = substring + input_text.substr(position + 1, input_text.size());
}

void ComponentInputbox::CheckOutputText()
{
	if (!output_timer.isStoped() && output_timer.ReadSec() >= 1) {
		output_text = input_text;
	}
	else {
		std::string substring = input_text.substr(0, position);
		output_text = substring + "|" + input_text.substr(position, input_text.size());
	}
}

void ComponentInputbox::ChangeResourceMaterial(ResourceMaterial* resource_mat)
{
	if (resource_material_sprite != nullptr) {
		App->resources->FreeResource(resource_material_sprite);
	}
	resource_material_sprite = resource_mat;
}

void ComponentInputbox::Inspector()
{
	ImGui::PushID(name.c_str());
	Component::Inspector();
	ComponentUI::Inspector();

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Sprite:");
	ImGui::Separator();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("File: ");

	ImGui::SameLine();

	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), resource_material_sprite != nullptr ? resource_material_sprite->file_name.c_str() : "None");
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
		ImGui::SetTooltip(resource_material_sprite != nullptr ? resource_material_sprite->full_file_name.c_str() : "");
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

	TextInspector();

	ImGui::PopID();
}

void ComponentInputbox::SetTrans(ComponentTransform2D* trans)
{
	this->trans = trans;
}

void ComponentInputbox::SetInOutput(std::string _in, std::string _out)
{
	input_text = _in;
	output_text = _out;
}

void ComponentInputbox::SetPos(int pos)
{
	this->position = pos;
}

void ComponentInputbox::SetActivity(bool active)
{
	text_input_activated = active;
}

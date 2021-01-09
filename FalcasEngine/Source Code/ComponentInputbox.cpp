#include "ComponentInputbox.h"
#include "Application.h"
#include "ComponentTransform2D.h"
#include "ModuleResources.h"
#include "ModuleSceneIntro.h"
#include "ResourceMesh.h"
#include "ModuleInput.h"
#include "External Libraries/ImGui/imgui.h"
#include "External Libraries/SDL/include/SDL_events.h"


ComponentInputbox::ComponentInputbox(GameObject* owner, ComponentTransform2D* trans) : ComponentUI(Component_Type::Inputbox, owner, "Input Box"), trans(trans)
{
}


ComponentInputbox::~ComponentInputbox()
{
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
	if (is_focused == false) {
		if (input_text == "") {
			output_text = initial_text;
		}
		return;
	}
	
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
	

}

bool ComponentInputbox::SaveComponent(JsonObj& obj)
{
	obj.AddString("Output", output_text.c_str());
	obj.AddString("Input", input_text.c_str());
	obj.AddFloat4x4("Matrix", trans->GetGlobalMatrix());
	obj.AddInt("Pos", position);
	obj.AddBool("Active", text_input_activated);
	SaveText(obj);
	//ARNAU: Sha de fer amb amb el timer?
	return true;
}

void ComponentInputbox::Initialization()
{
}

void ComponentInputbox::Render()
{
	if (!active)
		return;

	PrintText(_text, _size, _color, trans);

	if (!is_focused) {
		resource_mesh->Render((float*)&trans->GetGlobalMatrixTransposed(), nullptr, false, false, false, nullptr);
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

void ComponentInputbox::Inspector()
{
	ImGui::PushID(name.c_str());
	Component::Inspector();
	ComponentUI::Inspector();

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Initial text: ");
	ImGui::SameLine();
	ImGui::InputText("##1", &initial_text, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll);

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

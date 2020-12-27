#include "ComponentCheckbox.h"
#include "External Libraries/ImGui/imgui.h"


ComponentCheckbox::ComponentCheckbox(GameObject* owner) : Component(Component_Type::Checkbox, owner, "Checkbox")
{
}


ComponentCheckbox::~ComponentCheckbox()
{

}

void ComponentCheckbox::Update()
{
	/*for (int i = 0; i < App->scene_intro->resources_material_to_delete.size(); i++) {
		if (resource_material == App->scene_intro->resources_material_to_delete[i]) {
			resource_material = nullptr;
			return;
		}
	}*/
}

void ComponentCheckbox::Initialization()
{
}

void ComponentCheckbox::Render()
{
}

bool ComponentCheckbox::SaveComponent(JsonObj& obj)
{
	//obj.AddInt("Resource_ID", resource_material != nullptr ? resource_material->GetID() : 0);
	return true;
}

void ComponentCheckbox::Inspector()
{
	bool falsed = false;

	ImGui::PushID(name.c_str());
	Component::Inspector();


	ImGui::Separator();

	ImGui::PopID();
}

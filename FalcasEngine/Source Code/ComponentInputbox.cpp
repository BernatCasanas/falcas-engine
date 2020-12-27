#include "ComponentInputbox.h"
#include "External Libraries/ImGui/imgui.h"


ComponentInputbox::ComponentInputbox(GameObject* owner) : Component(Component_Type::Inputbox, owner, "Input Box")
{
}


ComponentInputbox::~ComponentInputbox()
{
	
}

void ComponentInputbox::Update()
{
	/*for (int i = 0; i < App->scene_intro->resources_material_to_delete.size(); i++) {
		if (resource_material == App->scene_intro->resources_material_to_delete[i]) {
			resource_material = nullptr;
			return;
		}
	}*/
}

bool ComponentInputbox::SaveComponent(JsonObj& obj)
{
	//obj.AddInt("Resource_ID", resource_material != nullptr ? resource_material->GetID() : 0);
	return true;
}

void ComponentInputbox::Inspector()
{
	bool falsed = false;

	ImGui::PushID(name.c_str());
	Component::Inspector();


	ImGui::Separator();

	ImGui::PopID();
}

void ComponentInputbox::Initialization()
{
}

void ComponentInputbox::Render()
{
}

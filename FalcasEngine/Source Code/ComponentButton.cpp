#include "ComponentButton.h"
#include "External Libraries/ImGui/imgui.h"


ComponentButton::ComponentButton(GameObject* owner) : Component(Component_Type::Button, owner, "Button")
{
}


ComponentButton::~ComponentButton()
{

}

void ComponentButton::Update()
{
	/*for (int i = 0; i < App->scene_intro->resources_material_to_delete.size(); i++) {
		if (resource_material == App->scene_intro->resources_material_to_delete[i]) {
			resource_material = nullptr;
			return;
		}
	}*/
}

void ComponentButton::Initialization()
{
}

void ComponentButton::Render()
{
}

bool ComponentButton::SaveComponent(JsonObj& obj)
{
	//obj.AddInt("Resource_ID", resource_material != nullptr ? resource_material->GetID() : 0);
	return true;
}

void ComponentButton::Inspector()
{
	bool falsed = false;

	ImGui::PushID(name.c_str());
	Component::Inspector();


	ImGui::Separator();

	ImGui::PopID();
}

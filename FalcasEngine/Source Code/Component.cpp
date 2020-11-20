#include "Component.h"
#include "External Libraries/ImGui/imgui.h"
#include "GameObject.h"


Component::Component(Component_Type type, GameObject* owner, std::string name): owner(owner), type(type), name(name)
{

}

Component::~Component()
{
	owner = nullptr;
}

void Component::Enable()
{
	active = true;
}

void Component::Update()
{
}

void Component::Disable()
{
	active = false;
}

void Component::Inspector()
{
	ImGui::Checkbox("Active", &active);
	if (type == Component_Type::Transform)
		return;
	ImGui::SameLine();
	if (ImGui::Button("Delete Component")) {
		to_delete = true;
	}
}

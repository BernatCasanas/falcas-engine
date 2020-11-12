#include "Component.h"
#include "External Libraries/ImGui/imgui.h"
#include "GameObject.h"


Component::Component(Component_Type type, GameObject* owner)
{
	this->type = type;
	this->owner = owner;
	this->active = true;
	this->name = "";
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
}

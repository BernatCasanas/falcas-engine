#pragma once
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
	if (owner != nullptr) {
		std::vector<Component*>::iterator it = owner->components.begin();
		for (int i = 0; it != owner->components.end(); ++it, i++) {
			if (owner->components.at(i)->name == name) {
				owner->components.erase(it);
				break;
			}
		}
	}
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

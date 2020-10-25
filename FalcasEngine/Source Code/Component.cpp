#pragma once
#include "Component.h"

Component::Component(Component_Type type, GameObject* owner)
{
	this->type = type;
	this->owner = owner;
	this->active = true;
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

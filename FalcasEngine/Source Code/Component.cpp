#pragma once
#include "Component.h"


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

bool Component::Inspector(Gui_Type& type, int& index, std::string& info, bool*& checked, float*& number, bool& same_line, bool& separator_in_column, bool& next_column,
	int& num_columns, float& width, float4& color)
{
	switch (index)
	{
	case 0:
		type = Gui_Type::CheckBox;
		checked = &active;
		same_line = separator_in_column = next_column = false;
		num_columns = 0;
		width = 50;
		info = "Active";
		index++;
		return false;
		break;
	default:
		return false;
		break;
	
	}
}

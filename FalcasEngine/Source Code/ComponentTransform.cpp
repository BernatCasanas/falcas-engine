#pragma once
#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(GameObject* owner, float3 position, Quat rotation, float3 size) :Component(Component_Type::Transform, owner)
{
	this->position = position;
	this->rotation = rotation;
	this->size = size;
	name = "Transform";
}

ComponentTransform::~ComponentTransform()
{
}

float3 ComponentTransform::GetPosition()
{
	return position;
}

Quat ComponentTransform::GetRotation()
{
	return rotation;
}

float3 ComponentTransform::GetSize()
{
	return size;
}

bool ComponentTransform::Inspector(Gui_Type& type, int& index, std::string& info, bool*& checked, float& number, bool& same_line, std::string& info2_for_tooltip)
{
	switch (index)
	{
	case 0:
		Component::Inspector(type,index,info,checked,number,same_line,info2_for_tooltip);
		break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
		switch (index)
		{
		case 1:
		case 5:
		case 9:
			switch (index)
			{
			case 1:
				info = "Position";
				break;
			case 5:
				info = "Rotation";
				break;
			case 9:
				info = "Scale";
				break;
			}
			same_line = false;
			break;
		case 2:
		case 3:
		case 4:
		case 6:
		case 7:
		case 8:
		case 10:
		case 11:
		case 12:
			switch (index)
			{
			case 2:
			case 6:
			case 10:
				info = "X";
				break;
			case 3:
			case 7:
			case 11:
				info = "Y";
				break;
			case 4:
			case 8:
			case 12:
				info = "Z";
				break;
			}
			same_line = true;
			break;
		}
		type = Gui_Type::Text;
		info2_for_tooltip = "";
		index++;
		break;
	default:
		return false;
		break;
	}
	return true;
}

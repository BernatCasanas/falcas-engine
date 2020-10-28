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

float3 ComponentTransform::GetRotation(bool convert)
{
	float3 rotation_euler;
	if (rotation.x * rotation.y + rotation.z * rotation.w == 0.5) {
		rotation_euler.x = 0;
		rotation_euler.z = 2 * atan2(rotation.x, rotation.w);
	}
	else if (rotation.x* rotation.y + rotation.z * rotation.w == -0.5) {
		rotation_euler.x = 0;
		rotation_euler.z = -2 * atan2(rotation.x, rotation.w);
	}
	else {
		rotation_euler.x = atan2(2 * rotation.x * rotation.w - 2 * rotation.y * rotation.z, 1 - 2 * pow(rotation.x, 2) - 2 * pow(rotation.z, 2));
		rotation_euler.z = atan2(2 * rotation.y * rotation.w - 2 * rotation.x * rotation.z, 1 - 2 * pow(rotation.y, 2) - 2 * pow(rotation.z, 2));
	}
	rotation_euler.y = asin(2 * rotation.x * rotation.y + 2 * rotation.z * rotation.w);
	
	return rotation_euler;
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
	case 4:
	case 6:
	case 8:
	case 9:
	case 11:
	case 13:
	case 15:
	case 16:
	case 18:
	case 20:
		switch (index)
		{
		case 1:
		case 8:
		case 15:
			switch (index)
			{
			case 1:
				info = "Position";
				break;
			case 8:
				info = "Rotation";
				break;
			case 15:
				info = "Scale";
				break;
			}
			same_line = false;
			break;
		case 2:
		case 4:
		case 6:
		case 9:
		case 11:
		case 13:
		case 16:
		case 18:
		case 20:
			switch (index)
			{
			case 2:
			case 9:
			case 16:
				info = "X";
				break;
			case 4:
			case 11:
			case 18:
				info = "Y";
				break;
			case 6:
			case 13:
			case 20:
				info = "Z";
				break;
			}
			same_line = true;
			break;
		}
		type = Gui_Type::Text;
		index++;
		break;
	case 3:
	case 5:
	case 7:
	case 10:
	case 12:
	case 14:
	case 17:
	case 19:
	case 21:
		switch (index)
		{
		case 3:
			number = position.x;
			break;
		case 5:
			number = position.y;
			break;
		case 7:
			number = position.z;
			break;
		case 10: 
			number = GetRotation(true).x;
			break;
		case 12:
			number = GetRotation(true).y;
			break;
		case 14:
			number = GetRotation(true).z;
			break;
		case 17:
			number = size.x;
			break;
		case 19:
			number = size.y;
			break;
		case 21:
			number = size.z;
			break;
		}
		type = Gui_Type::SliderFloat;
		info = "";
		same_line = true;
		index++;
		break;
	default:
		return false;
		break;
	}
	info2_for_tooltip = "";
	return true;
}

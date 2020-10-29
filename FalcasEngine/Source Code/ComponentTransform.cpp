#pragma once
#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(GameObject* owner, float3 position, Quat rotation, float3 size) :Component(Component_Type::Transform, owner)
{
	this->position = position;
	this->rotation = rotation;
	this->size = size;
	euler = QuaternionToEuler(rotation);
	name = "Transform";
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Update()
{
	rotation = EulerToQuaternion(euler);
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
	return QuaternionToEuler(rotation);
}

float3 ComponentTransform::GetSize()
{
	return size;
}

float3 ComponentTransform::QuaternionToEuler(Quat q)
{
	float3 rotation_euler;
	if (rotation.x * rotation.y + rotation.z * rotation.w == 0.5) {
		rotation_euler.x = 0;
		rotation_euler.z = 2 * atan2(rotation.x, rotation.w);
	}
	else if (rotation.x * rotation.y + rotation.z * rotation.w == -0.5) {
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

Quat ComponentTransform::EulerToQuaternion(float3 eu)
{
	Quat q;
	q.w = cos(eu.z / 2) * cos(eu.y / 2) * cos(eu.x / 2) - sin(eu.z / 2) * sin(eu.y / 2) * sin(eu.x / 2);
	q.x = sin(eu.z / 2) * sin(eu.y / 2) * cos(eu.x / 2) + cos(eu.z / 2) * cos(eu.y / 2) * sin(eu.x / 2);
	q.y = sin(eu.z / 2) * cos(eu.y / 2) * cos(eu.x / 2) + cos(eu.z / 2) * sin(eu.y / 2) * sin(eu.x / 2);
	q.z = cos(eu.z / 2) * sin(eu.y / 2) * cos(eu.x / 2) - sin(eu.z / 2) * cos(eu.y / 2) * sin(eu.x / 2);
	return q;
}

bool ComponentTransform::Inspector(Gui_Type& type, int& index, std::string& info, bool*& checked, float*& number, bool& same_line, bool& separator_in_column, bool& next_column,
	int& num_columns, float& width, float4& color)
{
	info = "";
	same_line = separator_in_column = next_column = false;
	num_columns = 1;
	width = 50;
	switch (index)
	{
	case 0:
		Component::Inspector(type,index,info,checked,number,same_line, separator_in_column, next_column, num_columns, width, color);
		break;
	case 3:
	case 4:
	case 6:
	case 8:
	case 10:
	case 11:
	case 13:
	case 15:
	case 17:
	case 18:
	case 20:
	case 22:
		switch (index)
		{
		case 3:
		case 10:
		case 17:
			switch (index)
			{
			case 3:
				info = "Position";
				break;
			case 10:
				info = "Rotation";
				next_column = true;
				break;
			case 17:
				info = "Scale";
				next_column = true;
				break;
			}
			break;
		case 4:
		case 6:
		case 8:
		case 11:
		case 13:
		case 15:
		case 18:
		case 20:
		case 22:
			switch (index)
			{
			case 4:
			case 11:
			case 18:
				info = "X";
				break;
			case 6:
			case 13:
			case 20:
				info = "Y";
				break;
			case 8:
			case 15:
			case 22:
				info = "Z";
				break;
			}
			next_column = true;
			break;
		}
		type = Gui_Type::Text;
		index++;
		break;
	case 5:
	case 7:
	case 9:
	case 12:
	case 14:
	case 16:
	case 19:
	case 21:
	case 23:
		switch (index)
		{
		case 5:
			number = &position.x;
			break;
		case 7:
			number = &position.y;
			break;
		case 9:
			number = &position.z;
			break;
		case 12:
			number = &euler.x;
			break;
		case 14:
			number = &euler.y;
			break;
		case 16:
			number = &euler.z;
			break;
		case 19:
			number = &size.x;
			break;
		case 21:
			number = &size.y;
			break;
		case 23:
			number = &size.z;
			break;
		}
		type = Gui_Type::DragFloat;
		index++;
		same_line = true;
		break;
	case 1:
		type = Gui_Type::Separator;
		index++;
		break;
	case 2:
		num_columns = 4;
	case 24:
		type = Gui_Type::Column;
		index++;
		break;
	default:
		return false;
		break;
	}
	return true;
}

#pragma once
#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(GameObject* owner, float3 position, Quat rotation, float3 size) :Component(Component_Type::Transform, owner)
{
	this->position = position;
	this->rotation = rotation;
	this->size = size;
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

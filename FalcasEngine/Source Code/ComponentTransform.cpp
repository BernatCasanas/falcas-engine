#pragma once
#include "ComponentTransform.h"
#include "External Libraries/ImGui/imgui.h"

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

void ComponentTransform::Inspector()
{
	int index = 0;
	ImGui::PushID((name + std::to_string(index)).c_str());
	Component::Inspector();
	ImGui::PopID();
	index++;

	ImGui::PushID((name + std::to_string(index)).c_str());
	ImGui::Separator();
	ImGui::PopID();
	index++;

	ImGui::PushID((name + std::to_string(index)).c_str());
	ImGui::Columns(4, "", false);
	ImGui::PopID();
	index++;

	ImGui::PushID((name + std::to_string(index)).c_str());
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Position");
	ImGui::PopID();
	index++;

	ImGui::PushID((name + std::to_string(index)).c_str());
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("X");
	ImGui::PopID();
	index++;

	ImGui::PushID((name + std::to_string(index)).c_str());
	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::DragFloat("", &position.x, 0.01f);
	ImGui::PopItemWidth();
	ImGui::PopID();
	index++;

	ImGui::PushID((name + std::to_string(index)).c_str());
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Y");
	ImGui::PopID();
	index++;

	ImGui::PushID((name + std::to_string(index)).c_str());
	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::DragFloat("", &position.y, 0.01f);
	ImGui::PopItemWidth();
	ImGui::PopID();
	index++;

	ImGui::PushID((name + std::to_string(index)).c_str());
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Z");
	ImGui::PopID();
	index++;

	ImGui::PushID((name + std::to_string(index)).c_str());
	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::DragFloat("", &position.z, 0.01f);
	ImGui::PopItemWidth();
	ImGui::PopID();
	index++;

	ImGui::PushID((name + std::to_string(index)).c_str());
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Rotation");
	ImGui::PopID();
	index++;

	ImGui::PushID((name + std::to_string(index)).c_str());
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("X");
	ImGui::PopID();
	index++;

	ImGui::PushID((name + std::to_string(index)).c_str());
	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::DragFloat("", &euler.x, 0.01f);
	ImGui::PopItemWidth();
	ImGui::PopID();
	index++;

	ImGui::PushID((name + std::to_string(index)).c_str());
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Y");
	ImGui::PopID();
	index++;

	ImGui::PushID((name + std::to_string(index)).c_str());
	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::DragFloat("", &euler.y, 0.01f);
	ImGui::PopItemWidth();
	ImGui::PopID();
	index++;

	ImGui::PushID((name + std::to_string(index)).c_str());
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Z");
	ImGui::PopID();
	index++;

	ImGui::PushID((name + std::to_string(index)).c_str());
	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::DragFloat("", &euler.z, 0.01f);
	ImGui::PopItemWidth();
	ImGui::PopID();
	index++;

	ImGui::PushID((name + std::to_string(index)).c_str());
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Size");
	ImGui::PopID();
	index++;

	ImGui::PushID((name + std::to_string(index)).c_str());
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("X");
	ImGui::PopID();
	index++;

	ImGui::PushID((name + std::to_string(index)).c_str());
	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::DragFloat("", &size.x, 0.01f);
	ImGui::PopItemWidth();
	ImGui::PopID();
	index++;

	ImGui::PushID((name + std::to_string(index)).c_str());
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Y");
	ImGui::PopID();
	index++;

	ImGui::PushID((name + std::to_string(index)).c_str());
	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::DragFloat("", &size.y, 0.01f);
	ImGui::PopItemWidth();
	ImGui::PopID();
	index++;

	ImGui::PushID((name + std::to_string(index)).c_str());
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Z");
	ImGui::PopID();
	index++;

	ImGui::PushID((name + std::to_string(index)).c_str());
	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::DragFloat("", &size.z, 0.01f);
	ImGui::PopItemWidth();
	ImGui::PopID();
	index++;

	ImGui::PushID((name + std::to_string(index)).c_str());
	ImGui::Columns(1, "", false);
	ImGui::PopID();
}

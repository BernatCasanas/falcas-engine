#pragma once
#include "Component.h"
#include "External Libraries/MathGeoLib/include/Math/float3.h"
#include "External Libraries/MathGeoLib/include/Math/Quat.h"

class GameObject;


class ComponentTransform : public Component {
public:
	ComponentTransform(GameObject* owner, float3 position, Quat rotation, float3 size);
	~ComponentTransform();
	void Update() {};

	float3 GetPosition();
	Quat GetRotation();
	float3 GetSize();

private:
	float3 position;
	Quat rotation;
	float3 size;
};
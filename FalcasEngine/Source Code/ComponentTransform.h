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
	float3 GetRotation(bool convert);
	float3 GetSize();

	bool Inspector(Gui_Type& type, int& index, std::string& info, bool*& checked, float& number, bool& same_line, std::string& info2_for_tooltip);

private:
	float3 position;
	Quat rotation;
	float3 size;
};
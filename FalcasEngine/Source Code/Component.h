#pragma once
#include "Globals.h"
#include <string>
#include "External Libraries/MathGeoLib/include/Math/float4.h"
class GameObject;

enum class Component_Type {
	Uknown=-1,
	Transform,
	Mesh,
	Material,
};


class Component {
public:
	Component(Component_Type type, GameObject* owner);
	virtual ~Component();
	virtual void Enable();
	virtual void Update();
	virtual void Disable();
	virtual void Inspector();

	Component_Type type;
	bool active;
	GameObject* owner;
	std::string name;

};
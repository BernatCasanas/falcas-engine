#pragma once
#include "Globals.h"
#include <string>
class GameObject;

enum class Component_Type {
	Uknown = -1,
	Transform,
	Mesh,
	Material,
	Max,
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
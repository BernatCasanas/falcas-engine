#pragma once
#include "Globals.h"
#include <string>
class GameObject;

enum class Component_Type {
	Uknown = -1,
	Transform,
	Mesh,
	Material,
	Camera,
	Max,
};


class Component {
public:
	Component(Component_Type type, GameObject* owner, std::string name = "");
	virtual ~Component();
	virtual void Enable();
	virtual void Update();
	virtual void Disable();
	virtual void Inspector();

	uint GetUUID();
	void SetUUID(uint id);

	Component_Type type;
	bool active = true;
	GameObject* owner;
	std::string name;
	bool to_delete = false;

private:
	uint uuid;

};
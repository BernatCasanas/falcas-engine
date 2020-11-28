#pragma once
#include "Globals.h"
#include <string>
#include "Json.h"

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
	virtual bool LoadComponent(JsonObj& obj) { return true; };
	virtual bool SaveComponent(JsonObj& obj) { return true; };

	uint GetUUID();
	void SetUUID();

	Component_Type type;
	bool active = true;
	GameObject* owner;
	std::string name;
	bool to_delete = false;

private:
	uint uuid;

};
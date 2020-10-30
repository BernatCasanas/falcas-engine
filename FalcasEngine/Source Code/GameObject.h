#pragma once
#include "Globals.h"
#include "External Libraries/MathGeoLib/include/Math/float3.h"
#include "External Libraries/MathGeoLib/include/Math/Quat.h"
#include <vector>
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

class Component;
enum class Component_Type;

class GameObject {
public:
	GameObject(int id);
	GameObject(int id, std::string name, GameObject* parent);
	GameObject(int id, std::string name, GameObject* parent, float3 position, Quat rotation, float3 size);
	~GameObject();

	Component* CreateComponent(Component_Type type);
	Component* GetComponent(Component_Type type);
	void AddComponentToGameObject(Component* component);
	std::string GetName() { return name; }
	bool CheckComponentType(Component_Type type);

public:
	bool active;
	std::vector<Component*> components;

	ComponentMaterial* material;
	ComponentMesh* mesh;
	ComponentTransform* transform;

	GameObject* parent;
	std::vector<GameObject*> children;
	std::string name;
	int id;
private:
};

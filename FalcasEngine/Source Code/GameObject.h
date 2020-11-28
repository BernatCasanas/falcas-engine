#pragma once
#include "Globals.h"
#include "External Libraries/MathGeoLib/include/Math/float3.h"
#include "External Libraries/MathGeoLib/include/Math/Quat.h"
#include "External Libraries/MathGeoLib/include/Geometry/AABB.h"
#include "External Libraries/MathGeoLib/include/Geometry/OBB.h"
#include <vector>
#include "Json.h"

class Component;
class ComponentTransform;
enum class Component_Type;

class GameObject {
public:
	GameObject(int id);
	GameObject(int id, std::string name, GameObject* parent);
	GameObject(int id, std::string name, GameObject* parent, float3 position, Quat rotation, float3 size);
	~GameObject();

	void Update();

	Component* CreateComponent(Component_Type type);
	Component* CreateComponent(Component_Type type, char* file);
	Component* GetComponent(Component_Type type) const;
	void DeleteComponent(Component_Type type);
	void AddComponentToGameObject(Component* component);
	std::string GetName() { return name; }
	bool HasComponentType(Component_Type type) const;
	void RemoveFromParent();
	void NewChild(GameObject* game_obj);
	void UpdateAABB();
	bool CheckParentRecursively(GameObject* game_object_to_check);
	bool IsInsideFrustumCulling();
	bool IsCulled() const;
	AABB GetAABB()const;

	uint GetUUID();
	void SetUUID();
	
	bool SaveGameObject(JsonObj& obj);

public:
	bool active=true;
	std::vector<Component*> components;

	GameObject* parent;
	std::vector<GameObject*> children;
	std::string name;
	int id;
	bool to_delete = false;
private:
	ComponentTransform* trans = nullptr;
	AABB aabb;
	OBB obb;
	bool culled = false;
	uint uuid;
};

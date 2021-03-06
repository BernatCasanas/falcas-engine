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
class ComponentTransform2D;
class ResourceModel;
enum class Component_Type;

class GameObject {
public:
	GameObject(int id, bool is_ui = false);
	GameObject(int id, std::string name, GameObject* parent, bool is_ui = false);
	GameObject(int id, std::string name, GameObject* parent, float3 position, Quat rotation, float3 size, bool is_ui = false);
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
	bool IsUI() const { return is_ui; } 
	uint GetComponentsSize() const { return components.size(); }
	AABB GetAABB()const;
	void RemoveResourceModel();

	uint GetUUID();
	void SetUUID();
	void SetUUID(uint uuid);
	
	bool SaveGameObject(JsonObj& obj);

public:
	bool active=true;
	std::vector<Component*> components;

	GameObject* parent;
	std::vector<GameObject*> children;
	std::string name;
	int id;
	bool to_delete = false;
	ResourceModel* resource_model= nullptr;
private:
	ComponentTransform* trans = nullptr;
	ComponentTransform2D* trans2D = nullptr;
	bool is_ui = false;
	AABB aabb;
	OBB obb;
	bool culled = false;
	uint uuid = 0;
};

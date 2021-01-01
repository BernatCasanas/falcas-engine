#pragma once
#include "Component.h"

class GameObject;
class ResourceMesh;

class ComponentUI : public Component{
public:
	ComponentUI(Component_Type type, GameObject* owner, std::string name = "", bool has_aabb = false);
	virtual ~ComponentUI();
	
	virtual void Render();

	ResourceMesh* GetResourceMesh() const { return resource_mesh; }
	bool HasAABB() const { return has_aabb; }

protected:
	ResourceMesh* resource_mesh = nullptr;
	bool has_aabb = false;
};
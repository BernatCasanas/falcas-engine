#pragma once
#include "Component.h"

class GameObject;
class ResourceMesh;

class ComponentUI : public Component{
public:
	ComponentUI(Component_Type type, GameObject* owner, std::string name = "");
	virtual ~ComponentUI();
	
	virtual void Render();

	ResourceMesh* GetResourceMesh() const { return resource_mesh; }

	void GetPlane();

protected:
	ResourceMesh* resource_mesh = nullptr;
};
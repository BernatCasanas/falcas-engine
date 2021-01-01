#pragma once
#include "Component.h"
#include "External Libraries/MathGeoLib/include/Geometry/Triangle.h"

class GameObject;
class ResourceMesh;

class ComponentUI : public Component{
public:
	ComponentUI(Component_Type type, GameObject* owner, std::string name = "");
	virtual ~ComponentUI();
	
	virtual void Render();

	ResourceMesh* GetResourceMesh() const { return resource_mesh; }

	void CheckMouseHovering();
	void UpdateTriangles();

protected:
	ResourceMesh* resource_mesh = nullptr;
	Triangle triangle1;
	Triangle triangle2;
	bool is_mouse_hover = false;
};
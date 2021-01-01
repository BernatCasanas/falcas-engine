#pragma once
#include "Component.h"
#include "External Libraries/MathGeoLib/include/Geometry/Triangle.h"

class GameObject;
class ResourceMesh;
class Module;

class ComponentUI : public Component{
public:
	ComponentUI(Component_Type type, GameObject* owner, std::string name = "");
	virtual ~ComponentUI();
	
	virtual void Render();

	void OnTriggered(ComponentUI* component);

	ResourceMesh* GetResourceMesh() const { return resource_mesh; }

	bool CheckMouseHovering();
	void UpdateTriangles();
	void IsClicked();
	void StoppedClicking();

protected:
	ResourceMesh* resource_mesh = nullptr;
	Triangle triangle1;
	Triangle triangle2;
	bool is_mouse_hover = false;
	bool is_clicked = false;
	bool is_focusable = true;
	bool is_clicked_first_frame = false;
private:
	Module* listener = nullptr;
};
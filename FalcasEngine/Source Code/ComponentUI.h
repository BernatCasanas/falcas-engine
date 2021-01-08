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
	virtual void Update();

	void OnTriggered(ComponentUI* component);

	ResourceMesh* GetResourceMesh() const { return resource_mesh; }
	bool GetIfIsFocused() const { return is_focused; }
	void SetIfIsFocused(bool is_focused) { this->is_focused = is_focused; }

	bool CheckMouseHovering();
	void UpdateTriangles();
	void IsClicked(bool clicked_with_mouse = true);
	void StoppedClicking(bool clicked_with_mouse = true);

	virtual void Inspector();

public:
	int id_vector_uis;
	int layer_of_ui = 0;
protected:
	ResourceMesh* resource_mesh = nullptr;
	Triangle triangle1;
	Triangle triangle2;
	bool is_mouse_hover = false;
	bool is_clicked = false;
	bool is_focusable = true;
	bool is_clicked_first_frame = false;
	bool is_clicked_with_enter = false;
	bool is_focused = false;
	bool is_draggable = false;
private:
	Module* listener = nullptr;
};
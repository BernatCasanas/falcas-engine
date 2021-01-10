#pragma once
#include "Module.h"

class GameObject;

class ModuleUI : public Module
{
public:

	ModuleUI(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleUI();

	// Called when before render is available
	bool Init();

	// Call before first frame
	bool Start();

	// Called before all Updates
	update_status PreUpdate(float dt);


	void RenderUI();
	bool CheckHover();
	void MouseClicked(bool clicked_with_mouse = true);
	void MouseStoppedClicking(bool clicked_with_mouse = true);
	void TabEntered();

	void DeleteUI(int id_ui);

	void AddLayer();
	void DeleteLayer();
	

private:
	int layers_with_ui_blocking_selecting = 0;
	
	

public:
	uint mesh_plane_id;
	std::vector <GameObject*> UIs;
	int focus_ui_id = -1;

};

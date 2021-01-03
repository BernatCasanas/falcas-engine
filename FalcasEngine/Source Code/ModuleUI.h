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

	// Called before all Updates
	update_status Update(float dt);

	// Called after all Updates
	update_status PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	void RenderUI(GameObject* game_obj);
	bool CheckHover(GameObject* game_obj, bool is_hovering = false);
	void MouseClicked(GameObject* game_obj, bool clicked_with_mouse = true);
	void MouseStoppedClicking(GameObject* game_obj, bool clicked_with_mouse = true);
	

	void ChangeDebug();

	void ChangeFocus();

	void DeleteFocus();


	void ClearUI();


	void WorkWithTextInput(std::string text);

	void DeactivateButtons();

	void ActivateButtons();


private:
	//std::list <UI*> UIs;
	

	
	//j1Timer minimap_feedback_timer;

	//j1Timer animation_win_lose_timer;
	//fPoint global_pos;

public:
	uint mesh_plane_id;

};

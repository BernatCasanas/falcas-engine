#pragma once
#include "Module.h"


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

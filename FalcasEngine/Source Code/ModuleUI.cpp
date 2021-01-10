#include "ModuleUI.h"
#include "Application.h"
#include "ModuleResources.h"
#include "FileSystem.h"
#include "ResourceModel.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"
#include "ComponentUI.h"
#include "ModuleCamera3D.h"
#include "ComponentTransform2D.h"
#include "aClock.h"
#include "External Libraries/SDL/include/SDL_scancode.h"

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled, "moduleUI")
{
	
}

// Destructor
ModuleUI::~ModuleUI()
{}

// Called before render is available
bool ModuleUI::Init()
{
	bool ret = true;



	return ret;
}

// Called before the first frame
bool ModuleUI::Start()
{

	char* buffer;
	App->filesystem->LoadPath("Assets/Icons (read_only)/plane.obj.meta", &buffer);
	JsonObj plane_obj(buffer);
	ResourceModel* model_plane = (ResourceModel*)App->resources->RequestResource(plane_obj.GetInt("ID"));
	delete[] buffer;
	plane_obj.CleanUp();
	mesh_plane_id = model_plane->mesh_keys.back();
	App->resources->FreeResource(model_plane);
	return true;
}

// Update all guis
update_status ModuleUI::PreUpdate(float dt)
{
	if (!Time::gameTimer.active) {
		return update_status::UPDATE_CONTINUE;
	}
	bool is_mouse_hovering_ui= CheckHover();
	if (layers_with_ui_blocking_selecting > 0) {
		App->camera->stop_selecting = false;
	}
	else if (!App->camera->stop_selecting) {
		App->camera->stop_selecting = is_mouse_hovering_ui;
	}
	if (App->input->GetMouseButton(1) == KEY_DOWN|| App->input->GetMouseButton(1) == KEY_REPEAT) {
		MouseClicked();
	}
	else if (App->input->GetMouseButton(1) == KEY_UP) {
		MouseStoppedClicking();
	}
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_REPEAT) {
		MouseClicked(false);
	}
	else if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP) {
		MouseStoppedClicking(false);
	}
	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN) {
		TabEntered();
	}
	
	return update_status::UPDATE_CONTINUE;
}


void ModuleUI::RenderUI()
{
	GameObject* game_obj;
	for (int i = 0; i < UIs.size(); i++) {
		game_obj = UIs[i];
		if (!game_obj->active || !game_obj->IsUI() || game_obj->GetComponentsSize() <= 1)
			continue;
		((ComponentUI*)game_obj->components[1])->Render();
	}
	
}

bool ModuleUI::CheckHover()
{
	bool is_hovering = false;
	GameObject* game_obj;
	for (int i = 0; i < UIs.size(); i++) {
		game_obj = UIs[i];
		if (!game_obj->active || !game_obj->IsUI() || game_obj->GetComponentsSize() <= 1)
			continue;
		ComponentUI* ui = (ComponentUI*)game_obj->components[1];
		if (ui->layer_of_ui != 0 || !ui->CheckMouseHovering())
			continue;
		is_hovering = true;
	}
	return is_hovering;
}

void ModuleUI::MouseClicked(bool clicked_with_mouse)
{
	GameObject* game_obj;
	for (int i = 0; i < UIs.size(); i++) {
		game_obj = UIs[i];
		if (!game_obj->active || !game_obj->IsUI() || game_obj->GetComponentsSize() <= 1)
			continue;
		((ComponentUI*)game_obj->components[1])->IsClicked(clicked_with_mouse);
	}
}

void ModuleUI::MouseStoppedClicking(bool clicked_with_mouse)
{
	GameObject* game_obj;
	for (int i = 0; i < UIs.size(); i++) {
		game_obj = UIs[i];
		if (!game_obj->active || !game_obj->IsUI() || game_obj->GetComponentsSize() <= 1)
			continue;
		((ComponentUI*)game_obj->components[1])->StoppedClicking(clicked_with_mouse);
	}
}

void ModuleUI::TabEntered()
{
	GameObject* game_obj;
	for (int i = focus_ui_id + 1; i < UIs.size(); i++) {
		game_obj = UIs[i];
		if (i < 0 || !game_obj->active || !game_obj->IsUI() || game_obj->GetComponentsSize() <= 1)
			continue;
		ComponentUI* ui = (ComponentUI*)game_obj->components[1];
		if (ui->layer_of_ui != 0)
			continue;
		if(focus_ui_id>=0){
			((ComponentUI*)UIs[focus_ui_id]->components[1])->SetIfIsFocused(false);
		}
		focus_ui_id = i;
		ui->SetIfIsFocused(true);
		return;
	}
	if (focus_ui_id <= 0)
		return;
	for (int i = 0; i < focus_ui_id; i++) {
		game_obj = UIs[i];
		if (!game_obj->active || !game_obj->IsUI() || game_obj->GetComponentsSize() <= 1)
			continue;
		ComponentUI* ui = (ComponentUI*)game_obj->components[1];
		if (ui->layer_of_ui != 0)
			continue;
		if (focus_ui_id >= 0) {
			((ComponentUI*)UIs[focus_ui_id]->components[1])->SetIfIsFocused(false);
		}		focus_ui_id = i;
		ui->SetIfIsFocused(true);
		return;
	}
}

void ModuleUI::DeleteUI(int id_ui)
{
	std::vector<GameObject*>::iterator it = UIs.begin();
	for (int i = 0; i < UIs.size(); i++) {
		if (i < id_ui) {
			it++;
		}
		else if (i > id_ui + 1) {
			((ComponentUI*)UIs[i]->components[1])->id_vector_uis--;
		}
	}
	UIs.erase(it);
}

void ModuleUI::AddLayer()
{
	layers_with_ui_blocking_selecting++;
	for (int i = 0; i < UIs.size(); i++) {
		ComponentUI* ui = (ComponentUI*)UIs[i]->components[1];
		ui->layer_of_ui++;
		((ComponentTransform2D*)UIs[i]->components[0])->UpdateZ();
	}
}

void ModuleUI::DeleteLayer()
{
	if (layers_with_ui_blocking_selecting > 0)
		layers_with_ui_blocking_selecting--;
	for (int i = 0; i < UIs.size(); i++) {
		ComponentUI* ui = (ComponentUI*)UIs[i]->components[1];
		ui->layer_of_ui--;
		((ComponentTransform2D*)UIs[i]->components[0])->UpdateZ();
	}
}


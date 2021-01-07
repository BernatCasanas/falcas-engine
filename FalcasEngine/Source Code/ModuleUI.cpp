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
	//LOG("Loading GUI atlas");
	bool ret = true;

	/*atlas_file_name_num_0 = conf.child("atlas_0").attribute("file").as_string("");
	atlas_file_name_num_1 = conf.child("atlas_1").attribute("file").as_string("");
	atlas_file_name_num_2 = conf.child("atlas_2").attribute("file").as_string("");
	active = false;*/

	return ret;
}

// Called before the first frame
bool ModuleUI::Start()
{
	/*atlas_num_0 = App->tex->Load(atlas_file_name_num_0.c_str());
	atlas_num_1 = App->tex->Load(atlas_file_name_num_1.c_str());
	atlas_num_2 = App->tex->Load(atlas_file_name_num_2.c_str());
	for (int i = 0; i < 10; i++) {
		sfx_UI[i] = 0;
	}
	cursor_tex = App->tex->Load("gui/cursors.png");

	winlose_tex = App->tex->Load("gui/WinLoseBackground.png");*/
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
	/*bool mouse = false;
	lockClick = false;
	int count = 0;
	if (App->input->GetMouseButtonDown(1) == KEY_DOWN || App->input->GetMouseButtonDown(1) == KEY_REPEAT)
	{
		for (int i = UIs.size(); i >= 0 && mouse == false; i--)
		{
			std::list<UI*>::iterator it = UIs.begin();
			std::advance(it, i);

			if (it._Ptr->_Myval != UIs.end()._Ptr->_Myval)
			{
				mouse = it._Ptr->_Myval->CheckMouse();
			}
			if (mouse == true)count = i;
		}
	}
	if (mouse == true)
	{
		std::list<UI*>::iterator it = UIs.begin();
		std::advance(it, count);

		if (it._Ptr->_Myval != UIs.end()._Ptr->_Myval && it._Ptr->_Myval->focus == true)
			it._Ptr->_Myval->Move();
	}
	for (std::list<UI*>::iterator it = UIs.begin(); it != UIs.end(); it++)
	{
		it._Ptr->_Myval->PreUpdate();
	}*/
	return update_status::UPDATE_CONTINUE;
}

// Called after all Updates
update_status ModuleUI::Update(float dt)
{
	//App->renderer3D->RenderUI();
	/*for (std::list<UI*>::iterator it = UIs.begin(); it != UIs.end(); it++)
	{
		it._Ptr->_Myval->Update(dt);
	}*/

	return update_status::UPDATE_CONTINUE;
}

// Called after all Updates
update_status ModuleUI::PostUpdate(float dt)
{
	//if (App->console->console_active == true) {
	//	for (std::list<UI*>::iterator it = UIs.begin(); it != UIs.end(); it++)
	//	{
	//		if (it._Ptr->_Myval->GetPriority() != 2)
	//			it._Ptr->_Myval->PostUpdate();
	//	}
	//	for (std::list<UI*>::iterator it = UIs.begin(); it != UIs.end(); it++)
	//	{
	//		if (it._Ptr->_Myval->GetPriority() == 2)
	//			it._Ptr->_Myval->PostUpdate();
	//	}
	//}
	//else {
	//	for (std::list<UI*>::iterator it = UIs.begin(); it != UIs.end(); it++)
	//	{
	//		it._Ptr->_Myval->PostUpdate();
	//	}
	//}

	//iPoint rect_position = App->minimap->WorldToMinimap(-App->render->camera.x, -App->render->camera.y);
	//if (App->minimap->active == true)
	//	App->render->DrawQuad({ rect_position.x, rect_position.y, (int)(App->render->camera.w * App->minimap->scale),(int)(App->render->camera.h * App->minimap->scale) }, 255, 255, 255, 255,
	//		false, false);
	//if (App->minimap->show_damage_area == true) {
	//	App->minimap->show_damage_area = false;
	//	minimap_feedback_timer.Start();
	//}
	//if (minimap_feedback_timer.ReadSec() <= 1)
	//	App->render->DrawQuad({ App->minimap->damage_area.x - 5,App->minimap->damage_area.y - 5,10,10 }, 255, 255, 0, 255, false, false);


	////Show cursor ------------------------------
	//int x, y;
	//App->input->GetMousePosition(x, y);
	//iPoint p = App->render->ScreenToWorld(x, y);
	//SDL_Rect sec = { 0, 0, 22, 32 };
	//if (cursor_move == true && App->scene->active == true && App->scene->paused_game == false)
	//	sec = { 162,0,36,36 };
	//if (cursor_attack == true && App->scene->active == true && App->scene->paused_game == false)
	//	sec = { 216,0,35,33 };
	//if (cursor_heal == true && App->scene->active == true && App->scene->paused_game == false)
	//	sec = { 324,0,27,32 };

	//cursor_size = { sec.w,sec.h };
	//p = App->render->ScreenToWorld(x, y);

	//App->render->Blit(cursor_tex, p.x, p.y, &sec);

	////Win or Lose Window
	//if (App->entityManager->getPlayer() != nullptr && App->entityManager->active == true) {
	//	if (App->entityManager->getPlayer()->player_win == true) {
	//		if (App->scene->isInTutorial == true) {
	//			DoWinOrLoseWindow(3, true);
	//		}
	//		else {
	//			if (App->entityManager->getPlayer()->player_type == CivilizationType::VIKING) {
	//				DoWinOrLoseWindow(1, true);
	//			}
	//			else {
	//				DoWinOrLoseWindow(2, true);
	//			}
	//		}
	//	}

	//	else if (App->entityManager->getPlayer()->player_lose == true) {
	//		App->entityManager->initCivilizations = true;
	//		if (App->entityManager->getPlayer()->player_type == CivilizationType::VIKING) {
	//			DoWinOrLoseWindow(1, false);
	//		}
	//		else {
	//			DoWinOrLoseWindow(2, false);
	//		}
	//	}
	//}


	return update_status::UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleUI::CleanUp()
{
	/*LOG("Freeing GUI");
	for (std::list<UI*>::iterator it = UIs.begin(); it != UIs.end(); it++)
	{
		it._Ptr->_Myval->CleanUp();
		delete it._Ptr->_Myval;
	}
	UIs.clear();

	if (atlas_num_0)
	{
		App->tex->UnLoad(atlas_num_0);
	}
	if (atlas_num_1)
	{
		App->tex->UnLoad(atlas_num_1);
	}
	if (atlas_num_2)
	{
		App->tex->UnLoad(atlas_num_2);
	}
	App->tex->UnLoad(cursor_tex);
	App->tex->UnLoad(winlose_tex);*/
	return true;
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


void ModuleUI::ChangeDebug()
{
	/*for (std::list<UI*>::iterator it = UIs.begin(); it != UIs.end(); it++)
	{
		it._Ptr->_Myval->debug = !it._Ptr->_Myval->debug;
	}*/
}

void ModuleUI::ChangeFocus()
{
	//bool exit = false;
	//bool focus = false;
	//int count = 0;
	//for (std::list<UI*>::iterator it = UIs.begin(); it != UIs.end() && exit == false; it++)
	//{

	//	bool focusable = it._Ptr->_Myval->CheckFocusable();
	//	if (focusable == true) {
	//		count++;
	//		if (focus == true) {
	//			it._Ptr->_Myval->focus = true;
	//			exit = true;
	//		}
	//		else {
	//			focus = it._Ptr->_Myval->focus;
	//			it._Ptr->_Myval->focus = false;
	//		}
	//	}
	//}
	//if (count > 0 && exit == false)
	//{
	//	for (std::list<UI*>::iterator it = UIs.begin(); it != UIs.end() && exit == false; it++)
	//	{
	//		bool focusable = it._Ptr->_Myval->CheckFocusable();
	//		if (focusable == true) {
	//			it._Ptr->_Myval->focus = true;
	//			exit = true;
	//		}
	//	}
	//}
}

void ModuleUI::DeleteFocus() {
	/*for (std::list<UI*>::iterator it = UIs.begin(); it != UIs.end(); it++)
	{
		it._Ptr->_Myval->focus = false;
	}*/
}

void ModuleUI::ClearUI()
{
	//UIs.clear();
}


void ModuleUI::ActivateButtons() {
	/*for (std::list<UI*>::iterator it = UIs.begin(); it != UIs.end(); it++)
	{
		if (it._Ptr->_Myval->type == Type::BUTTON) {
			ButtonUI* button = (ButtonUI*)it._Ptr->_Myval;
			button->front = true;
		}
	}*/
}



void ModuleUI::DeactivateButtons() {
	/*for (std::list<UI*>::iterator it = UIs.begin(); it != UIs.end(); it++)
	{
		if (it._Ptr->_Myval->type == Type::BUTTON) {
			ButtonUI* button = (ButtonUI*)it._Ptr->_Myval;
			button->front = false;
		}
	}*/
}

void ModuleUI::WorkWithTextInput(std::string text) {
	/*bool exit = false;
	for (std::list<UI*>::iterator it = UIs.begin(); it != UIs.end() && exit == false; it++)
	{
		if (it._Ptr->_Myval->type == Type::INPUT && it._Ptr->_Myval->focus == true)
		{
			TextInputUI* text_ui = (TextInputUI*)it._Ptr->_Myval;
			text_ui->ChangeLabel(text);
		}
	}*/
}
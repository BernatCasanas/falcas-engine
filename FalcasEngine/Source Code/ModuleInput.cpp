#include "Application.h"
#include "ModuleInput.h"
#include "ModuleCentralEditor.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleSceneIntro.h"
#include "FileSystem.h"
#include "External Libraries/SDL/include/SDL.h"
#include "GameObject.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "Importer.h"
#include "ModuleResources.h"
#include "Resource.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool start_enabled) : Module(app, start_enabled, "moduleInput")
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN) {
				keyboard[i] = KEY_UP;
				char entry[50];
				sprintf_s(entry, 50, "Key ID: %i\n", i);
				App->central_editor->input_list.push_back(strdup(entry));
				App->central_editor->need_scroll = true;
			}
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if (mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN) {
				mouse_buttons[i] = KEY_UP;
				char entry[50];
				sprintf_s(entry, 50, "Key ID: %i\n", i);
				App->central_editor->input_list.push_back(strdup(entry));
				App->central_editor->need_scroll = true;
			}
			else
				mouse_buttons[i] = KEY_IDLE;
		}

	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		App->central_editor->ProcessEvents(e);
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
			break;

			case SDL_DROPFILE:
			{
				switch (App->filesystem->GetTypeFile(e.drop.file)) {
				case FILE_TYPE::FBX:
				{
					App->resources->ImportFileToLibrary(e.drop.file, true);
					break;
				}
				case FILE_TYPE::DDS:
				{
					uint id = std::stoi(App->filesystem->GetFileName(e.drop.file, true));
					Resource* resource = App->resources->GetResource(id);
					if (resource == nullptr) {
						App->resources->ImportFileToLibrary(e.drop.file, true);
						break;
					}
					GameObject* game_object = App->scene_intro->game_object_selected;
					if (game_object == nullptr || game_object == App->scene_intro->root) {
						game_object = App->scene_intro->CreateGameObject(App->filesystem->GetFileName(resource->GetAssetsFile(), true), App->scene_intro->root);
					}
					ComponentMaterial* mat;
					if (game_object->HasComponentType(Component_Type::Material))
						mat = (ComponentMaterial*)App->scene_intro->game_object_selected->GetComponent(Component_Type::Material);
					else
						mat = (ComponentMaterial*)game_object->CreateComponent(Component_Type::Material);
					mat->ChangeResourceMaterial((ResourceMaterial*)App->resources->RequestResource(id));
					break;
				}
				case FILE_TYPE::MESH:
				{
					uint id = std::stoi(App->filesystem->GetFileName(e.drop.file, true));
					Resource* resource = App->resources->GetResource(id);
					if (resource == nullptr)
						break;
					
					GameObject* game_object = App->scene_intro->game_object_selected;
					if (game_object == nullptr || game_object == App->scene_intro->root) {
						game_object = App->scene_intro->CreateGameObject(App->filesystem->GetFileName(resource->GetAssetsFile(), true), App->scene_intro->root);
					}
					ComponentMesh *mesh;
					if (game_object->HasComponentType(Component_Type::Mesh))
						mesh = (ComponentMesh*)App->scene_intro->game_object_selected->GetComponent(Component_Type::Mesh);
					else
						mesh = (ComponentMesh*)game_object->CreateComponent(Component_Type::Mesh);
					mesh->ChangeResourceMesh((ResourceMesh*)App->resources->RequestResource(id));
					break;
				}
				case FILE_TYPE::PNG:
				case FILE_TYPE::TGA:
				App->resources->ImportFileToLibrary(e.drop.file, true);
				break;
				}
			}
			break;
		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

KEY_STATE ModuleInput::GetKey(int id) const
{
	return keyboard[id];
}


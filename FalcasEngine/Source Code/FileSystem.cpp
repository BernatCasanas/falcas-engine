#include "FileSystem.h"

FileSystem::FileSystem(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

FileSystem::~FileSystem()
{
}

GameObject* FileSystem::CreateGameObject(std::string name, GameObject* parent)
{
	GameObject* game_object = nullptr;
	name = CheckNameGameObject(name);
	game_object = new GameObject(App->scene_intro->id_gameobject, name, parent);
	if (game_object != nullptr) {
		App->scene_intro->id_gameobject++;
		if (parent != nullptr) {
			parent->children.push_back(game_object);
		}

	}
	return game_object;
}

GameObject* FileSystem::CreateGameObject(float3 position, Quat rotation, float3 size, std::string name, GameObject* parent)
{
	GameObject* game_object = nullptr;
	name = CheckNameGameObject(name);
	game_object = new GameObject(App->scene_intro->id_gameobject, name, parent, position, rotation, size);
	if (game_object != nullptr) {
		App->scene_intro->id_gameobject++;
		if (parent != nullptr) {
			parent->children.push_back(game_object);
		}
	}
	return game_object;
}

GameObject* FileSystem::SearchGameObject(int id, GameObject* game_obj)
{
	GameObject* game_object = nullptr;
	if (game_obj->id == id) {
		return game_obj;
	}
	for (int i = 0; i < game_obj->children.size() && game_object == nullptr; i++) {
		game_object = SearchGameObject(id, game_obj->children.at(i));
	}
	return game_object;
}

std::string FileSystem::CheckNameGameObject(std::string name, bool numbered, int digits)
{
	bool stop = false;
	GameObject* game_object = nullptr;
	for (int i = 0; i <= App->scene_intro->id_gameobject && stop == false; i++) {
		game_object = SearchGameObject(i, App->scene_intro->root);
		if (game_object != nullptr && game_object->name == name) {
			stop = true;
		}
	}
	if (stop == true) {
		if (numbered) {
			name.pop_back();
			int number = name.back() - '0';
			if (number >= 9)
				name = NameGameObjectWhenMoreThan2Digits(name, digits);
			else {
				number++;
				name.pop_back();
				name.push_back(number + '0');
				name.push_back(')');
			}
		}
		else {
			name.push_back(' ');
			name.push_back('(');
			name.push_back('1');
			name.push_back(')');
		}
		name = CheckNameGameObject(name, true, digits);
	}
	return name;
}

std::string FileSystem::NameGameObjectWhenMoreThan2Digits(std::string name, int& digits)
{
	int number = 0;
	for (int i = 0; i < digits; i++) {
		number += (name.back() - '0') * (pow(10, i));
		name.pop_back();
	}
	number++;
	if (pow(10, digits) <= number) {
		digits++;
	}
	for (int i = digits - 1; i > 0; i--) {
		if (number > (pow(10, i)) - 1) {
			int n = number / pow(10, i);
			name.push_back(n + '0');
			number -= n * (pow(10, i));
		}
		else {
			name.push_back(0 + '0');
		}
	}

	name.push_back(number + '0');
	name.push_back(')');
	return name;
}

void FileSystem::CreateWhiteGameObject()
{
	CreateGameObject("GameObject", App->scene_intro->root);
}

void FileSystem::CreateCompleteGameObject(std::string name, GameObject* parent, std::string mesh_path, std::string material_path)
{
	GameObject* gameObject = App->filesystem->CreateGameObject(name, parent);
	gameObject->CreateComponent(Component_Type::Mesh);
	ComponentMaterial* mat = (ComponentMaterial*)gameObject->CreateComponent(Component_Type::Material);
	ComponentMesh* mesh = (ComponentMesh*)gameObject->GetComponent(Component_Type::Mesh);
	mat->LoadTexture(material_path);
	mesh->LoadMesh({ 0,0,0 }, mesh_path.c_str(), name, mat);
}

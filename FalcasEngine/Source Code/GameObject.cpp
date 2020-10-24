#include "GameObject.h"

GameObject::GameObject(int id, std::string name, GameObject* parent, bool active)
{
	this->active = active;
	this->children.clear();
	this->components.clear();
	this->name = name;
	this->parent = parent;
	this->id = id;
}

GameObject::GameObject(int id)
{
	this->active = true;
	this->children.clear();
	this->components.clear();
	this->name = "";
	this->parent = nullptr;
	this->id = id;
}

GameObject::~GameObject()
{
	if (parent != nullptr) {
		bool stop = false;
		std::vector<GameObject*>::iterator it = parent->children.begin();
		for (int i = 0; it != parent->children.end() && stop == false; ++it, i++) {
			if (parent->children.at(i)->id == id) {
				parent->children.erase(it);
				stop = true;
			}
		}
	}
	parent = nullptr;
	int size = children.size();
	for (int i = size-1; i >= 0; i++) {
		delete children.at(i);
	}
	children.clear();
	size = components.size();
	for (int i = size-1; i >= 0; i++) {
		delete components.at(i);
	}
	components.clear();
}

Component* GameObject::CreateComponent(Component_Type type)
{
	return nullptr;
}

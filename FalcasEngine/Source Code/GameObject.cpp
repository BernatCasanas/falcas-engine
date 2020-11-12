#include "ComponentMaterial.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"

GameObject::GameObject(int id) : name(""), parent(nullptr), id(id)
{
	CreateComponent(Component_Type::Transform);
}
GameObject::GameObject(int id, std::string name, GameObject* parent) : name(name), parent(parent), id(id)
{
	CreateComponent(Component_Type::Transform);
}

GameObject::GameObject(int id, std::string name, GameObject* parent, float3 position, Quat rotation, float3 size) : name(name), parent(parent), id(id)
{
	AddComponentToGameObject(new ComponentTransform(this, position, rotation, size));
}


GameObject::~GameObject()
{
	RemoveFromParent();
	parent = nullptr;
	int size = children.size();
	for (int i = size-1; i >= 0; i--) {
		delete children[i];
	}
	children.clear();
	size = components.size();
	for (int i = size - 1; i >= 0; i--) {
		delete components[i];
	}
	components.clear();
}

void GameObject::Update()
{
	if (active == true) {
		for (int i = 0; i < children.size(); i++) {
			if(children[i]->active == true)
				children[i]->Update();
		}
		for (int i = 0; i < components.size(); i++) {
			if(components[i]->active==true)
				components[i]->Update();
		}
	}
}

Component* GameObject::CreateComponent(Component_Type type)
{
	Component* component = nullptr;
	if (!HasComponentType(type)) {
		switch (type)
		{
		case Component_Type::Transform:
			component = new ComponentTransform(this, { 0,0,0 }, Quat::identity, { 1,1,1 });
			break;
		case Component_Type::Mesh:
			component = new ComponentMesh(this);
			break;
		case Component_Type::Material:
			component = new ComponentMaterial(this);
			break;
		}
		components.push_back(component);
	}
	return component;
}

Component* GameObject::CreateComponent(Component_Type type, char* file)
{
	Component* component = nullptr;
	if (!HasComponentType(type)) {
		switch (type)
		{
		case Component_Type::Material:
			component = new ComponentMaterial(this, file);
			break;
		}
		components.push_back(component);
	}
	return component;
}

Component* GameObject::GetComponent(Component_Type type) const
{
	Component* component = nullptr;
	bool stop = false;
	for (int i = 0; i < components.size()&& stop == false; i++) {
		if (components[i]->type == type) {
			stop = true;
			component = components[i];
		}
	}
	return component;
}

void GameObject::DeleteComponent(Component_Type type)
{
	if (!HasComponentType(type)) {
		return;
	}
	ComponentTransform* t = (ComponentTransform*)components.front();
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it) {
		if ((*it)->type == type) {
			delete (*it);
			components.erase(it);
			break;
		}
	}	
}

void GameObject::AddComponentToGameObject(Component* component)
{
	if (!HasComponentType(component->type)) {
		components.push_back(component);
	}
}

bool GameObject::HasComponentType(Component_Type type) const
{
	bool returned = false;
	for (int i = 0; i < components.size() && returned == false; i++) {
		if (components[i]->type == type)
			returned = true;
	}
	return returned;
}

void GameObject::RemoveFromParent()
{
	if (parent == nullptr) {
		return;
	}
	for (std::vector<GameObject*>::iterator it = parent->children.begin(); it != parent->children.end(); ++it) {
		if ((*it)->id == id) {
			parent->children.erase(it);
			break;
		}
	}
}


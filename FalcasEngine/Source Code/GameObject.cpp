#pragma once
#include "ComponentMaterial.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"

GameObject::GameObject(int id)
{
	this->active = true;
	this->children.clear();
	this->components.clear();
	this->name = "";
	this->parent = nullptr;
	this->id = id;
	CreateComponent(Component_Type::Transform);
}
GameObject::GameObject(int id, std::string name, GameObject* parent)
{
	this->active = true;
	this->children.clear();
	this->components.clear();
	this->name = name;
	this->parent = parent;
	this->id = id;
	CreateComponent(Component_Type::Transform);
}

GameObject::GameObject(int id, std::string name, GameObject* parent, float3 position, Quat rotation, float3 size)
{
	this->active = true;
	this->children.clear();
	this->components.clear();
	this->name = name;
	this->parent = parent;
	this->id = id;
	AddComponentToGameObject(new ComponentTransform(this, position, rotation, size));
}


GameObject::~GameObject()
{
	if (parent != nullptr) {
		std::vector<GameObject*>::iterator it = parent->children.begin();
		for (int i = 0; it != parent->children.end(); ++it, i++) {
			if (parent->children.at(i)->id == id) {
				parent->children.erase(it);
				break;
			}
		}
	}
	parent = nullptr;
	int size = children.size();
	for (int i = size-1; i >= 0; i--) {
		delete children.at(i);
	}
	children.clear();
	for (int i = 0; i < 3; i++) {
		DeleteComponent((Component_Type)i);
	}
	components.clear();
}

void GameObject::Update()
{
	if (active == true) {
		for (int i = 0; i < children.size(); i++) {
			if(children.at(i)->active == true)
				children.at(i)->Update();
		}
		for (int i = 0; i < components.size(); i++) {
			if(components.at(i)->active==true)
				components.at(i)->Update();
		}
	}
}

Component* GameObject::CreateComponent(Component_Type type)
{
	Component* component = nullptr;
	if (!CheckComponentType(type)) {
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
	if (!CheckComponentType(type)) {
		switch (type)
		{
		case Component_Type::Mesh:
			component = new ComponentMesh(this, file);
			break;
		case Component_Type::Material:
			component = new ComponentMaterial(this, file);
			break;
		}
		components.push_back(component);
	}
	return component;
}

Component* GameObject::GetComponent(Component_Type type)
{
	Component* component = nullptr;
	bool stop = false;
	for (int i = 0; i < components.size()&& stop == false; i++) {
		if (components.at(i)->type == type) {
			stop = true;
			component = components.at(i);
		}
	}
	return component;
}

void GameObject::DeleteComponent(Component_Type type)
{
	Component* c= GetComponent(type);
	if (c != nullptr)
		delete c;
}

void GameObject::AddComponentToGameObject(Component* component)
{
	if (!CheckComponentType(component->type)) {
		components.push_back(component);
	}
}

bool GameObject::CheckComponentType(Component_Type type)
{
	bool returned = false;
	for (int i = 0; i < components.size() && returned == false; i++) {
		if (components.at(i)->type == type)
			returned = true;
	}
	return returned;
}


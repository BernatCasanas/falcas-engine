#include "ComponentMaterial.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleCentralEditor.h"
#include "ModuleSceneIntro.h"
#include "FileSystem.h"
#include "Application.h"
#include "ResourceMesh.h"
#include "External Libraries/MathGeoLib/include/MathGeoLib.h"

GameObject::GameObject(int id) : name(""), parent(nullptr), id(id)
{
	trans= (ComponentTransform*)CreateComponent(Component_Type::Transform);
}
GameObject::GameObject(int id, std::string name, GameObject* parent) : name(name), parent(parent), id(id)
{
	trans= (ComponentTransform*)CreateComponent(Component_Type::Transform);
}

GameObject::GameObject(int id, std::string name, GameObject* parent, float3 position, Quat rotation, float3 size) : name(name), parent(parent), id(id)
{
	AddComponentToGameObject(new ComponentTransform(this, position, rotation, size));
	trans = (ComponentTransform*)GetComponent(Component_Type::Transform);
}


GameObject::~GameObject()
{
	if (App->scene_intro->game_object_selected == this)
		App->scene_intro->game_object_selected = nullptr;
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
	if (!active)
		return;

	if (trans->needed_to_update)
		UpdateAABB();
	
	if (id > 0 && App->renderer3D->camera_culling->GetIfIsFrustumCulling() && HasComponentType(Component_Type::Mesh) && !IsInsideFrustumCulling())
		culled = true;
	else culled = false;
	
	int component_to_delete = 0;
	for (int i = 0; i < components.size(); i++) {
		if(components[i]->active)
			components[i]->Update();
		if (components[i]->to_delete)
			component_to_delete++;
	}
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end() && component_to_delete > 0; ++it) {
		if ((*it)->to_delete == true) {
			std::vector<Component*>::iterator it_to_continue=it;
			--it_to_continue;
			delete (*it);
			components.erase(it);
			it = it_to_continue;
			component_to_delete--;

		}
	}
	for (int i = 0; i < children.size(); i++) {
		if (children[i]->to_delete) {
			App->scene_intro->game_objects_to_delete.push_back(children[i]->id);
			continue;
		}
		if (!children[i]->active) {
			continue;
		}
		if (culled)
			children[i]->culled;
		if (trans->needed_to_update_only_children) {
			children[i]->trans->SetMatricesWithNewParent(trans->GetGlobalMatrix());
		}
		if (trans->needed_to_update) {
			children[i]->trans->needed_to_update = true;
		}
		children[i]->Update();
	}
	trans->needed_to_update_only_children = false;
	trans->needed_to_update = false;
	if (App->central_editor->aabbs)
		App->renderer3D->aabbs.push_back(aabb);
	
}

Component* GameObject::CreateComponent(Component_Type type)
{
	Component* component = nullptr;
	if (!HasComponentType(type)) {
		switch (type)
		{
		case Component_Type::Transform:
			component = new ComponentTransform(this, { 0,0,0 }, Quat::identity, { 1,1,1 });
			component->name = "Transform";
			break;
		case Component_Type::Mesh:
			component = new ComponentMesh(this);
			component->name = "Mesh";
			break;
		case Component_Type::Material:
			component = new ComponentMaterial(this);
			component->name = "Material";
			break;
		case Component_Type::Camera:
			component = new ComponentCamera(this, trans);
			component->name = "Camera";
			break;
		}
		component->SetUUID();
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
		component->SetUUID();
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

void GameObject::NewChild(GameObject* game_obj)
{
	children.push_back(game_obj);
	game_obj->parent = this;
	game_obj->trans->SetMatricesWithNewParent(trans->GetGlobalMatrix());
}

void GameObject::UpdateAABB()
{
	if (!HasComponentType(Component_Type::Mesh))
		return;

	//obb
	ComponentMesh* mesh = (ComponentMesh*)GetComponent(Component_Type::Mesh);
	if (mesh == nullptr)
		return;
	//obb = mesh->GetAABB();
	ComponentTransform* trans = (ComponentTransform*)GetComponent(Component_Type::Transform);
	obb.Transform(trans->GetGlobalMatrix());

	//aabb
	aabb.SetNegativeInfinity();
	aabb.Enclose(obb);
}

bool GameObject::CheckParentRecursively(GameObject* game_object_to_check)
{
	if (parent == game_object_to_check)
		return true;
	else if (parent == nullptr)
		return false;
	return parent->CheckParentRecursively(game_object_to_check);
}

bool GameObject::IsInsideFrustumCulling()
{
	float3 vCorner;
	for (int p = 0; p < 6; ++p) {
		int outside = 0;
		for (int i = 0; i < 8; ++i) {
			vCorner = aabb.CornerPoint(i);
				if (App->renderer3D->camera_culling->frustum.GetPlane(p).IsOnPositiveSide(vCorner)) { 
					outside++;
				}
				
			}
		if (outside == 8)
			return false;
	}
	return true;
}

bool GameObject::IsCulled() const
{
	return culled;
}

AABB GameObject::GetAABB() const
{
	return aabb;
}

uint GameObject::GetUUID()
{
	return uuid;
}

void GameObject::SetUUID()
{
	uuid = LCG().Int();
}

void GameObject::SetUUID(uint uuid)
{
	this->uuid = uuid;
}

bool GameObject::SaveGameObject(JsonObj& obj)
{
	obj.AddInt("UUID", GetUUID());
	if (parent != nullptr) {;
		obj.AddInt("Parent UUID", parent->GetUUID());
	}
	obj.AddString("name", this->name.c_str());
	JsonArray transArr = obj.AddArray("Translation");
	transArr.AddInt(trans->GetPosition().x);
	transArr.AddInt(trans->GetPosition().y);
	transArr.AddInt(trans->GetPosition().z);

	transArr = obj.AddArray("Scale");
	transArr.AddInt(trans->GetSize().x);
	transArr.AddInt(trans->GetSize().y);
	transArr.AddInt(trans->GetSize().z);

	transArr = obj.AddArray("Rotation");
	transArr.AddInt(trans->GetRotation().x);
	transArr.AddInt(trans->GetRotation().y);
	transArr.AddInt(trans->GetRotation().z);

	transArr = obj.AddArray("Components");
	for (std::vector<Component*>::iterator it = components.begin(); it!=components.end(); it++) {
		JsonObj componentObject;
		transArr.AddObject(componentObject);
		componentObject.AddString("name", (*it)->name.c_str());
		(*it)->SaveComponent(componentObject);
	}
	return true;
}


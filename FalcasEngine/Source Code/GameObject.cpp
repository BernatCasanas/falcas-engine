#include "ComponentMaterial.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentTransform2D.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "ComponentImage.h"
#include "ComponentCheckbox.h"
#include "ComponentButton.h"
#include "ComponentInputbox.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleCentralEditor.h"
#include "ModuleSceneIntro.h"
#include "FileSystem.h"
#include "Application.h"
#include "ResourceMesh.h"
#include "ModuleResources.h"
#include "ResourceModel.h"
#include "External Libraries/MathGeoLib/include/MathGeoLib.h"
#include "ModuleCamera3D.h"

GameObject::GameObject(int id, bool is_ui) : name(""), parent(nullptr), id(id), is_ui(is_ui)
{
	if (is_ui) {
		trans2D = (ComponentTransform2D*)CreateComponent(Component_Type::Transform2D);
	}
	else {
		trans = (ComponentTransform*)CreateComponent(Component_Type::Transform);
	}
}
GameObject::GameObject(int id, std::string name, GameObject* parent, bool is_ui) : name(name), parent(parent), id(id), is_ui(is_ui)
{
	if (is_ui) {
		trans2D = (ComponentTransform2D*)CreateComponent(Component_Type::Transform2D);
	}
	else {
		trans = (ComponentTransform*)CreateComponent(Component_Type::Transform);
	}
}

GameObject::GameObject(int id, std::string name, GameObject* parent, float3 position, Quat rotation, float3 size, bool is_ui) : name(name), parent(parent), id(id), is_ui(is_ui)
{
	if (is_ui) {
		AddComponentToGameObject(new ComponentTransform2D(this, { position.x,position.y }, { rotation.x,rotation.y }, { size.x,size.y }));
		trans2D = (ComponentTransform2D*)GetComponent(Component_Type::Transform2D);
	}
	else {
		AddComponentToGameObject(new ComponentTransform(this, position, rotation, size));
		trans = (ComponentTransform*)GetComponent(Component_Type::Transform);
	}
}


GameObject::~GameObject()
{
	if (resource_model != nullptr && !App->resources->isResourcesMapEmpty()) {
		App->resources->FreeResource(resource_model);
	}
	resource_model = nullptr;
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
	for (int i = 0; i < App->scene_intro->resources_model_to_delete.size(); i++) {
		if (resource_model == App->scene_intro->resources_model_to_delete[i]) {
			App->scene_intro->game_objects_to_delete.push_back(id);
			break;
		}
	}

	if (!active)
		return;

	if (!is_ui && trans->needed_to_update)
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
		if (!is_ui) {
			if (trans->needed_to_update_only_children) {
				if (children[i]->is_ui) {
					children[i]->trans2D->SetMatricesWithNewParent(trans->GetGlobalMatrix());
				}
				else {
					children[i]->trans->SetMatricesWithNewParent(trans->GetGlobalMatrix());
				}
			}
			if (trans->needed_to_update) {
				if (children[i]->is_ui) {
					children[i]->trans2D->needed_to_update = true;
				}
				else {
					children[i]->trans->needed_to_update = true;
				}
			}
		}
		else {
			if (trans2D->needed_to_update_only_children) {
				if (children[i]->is_ui) {
					children[i]->trans2D->SetMatricesWithNewParent(trans2D->GetGlobalMatrix());
				}
				else {
					children[i]->trans->SetMatricesWithNewParent(trans2D->GetGlobalMatrix());
				}
			}
			if (trans2D->needed_to_update) {
				if (children[i]->is_ui) {
					children[i]->trans2D->needed_to_update = true;
				}
				else {
					children[i]->trans->needed_to_update = true;
				}
			}
		}
		children[i]->Update();
	}
	if (is_ui) {
		trans2D->needed_to_update_only_children = false;
		trans2D->needed_to_update = false;
	}
	else {
		trans->needed_to_update_only_children = false;
		trans->needed_to_update = false;
	}
	if (App->central_editor->aabbs)
		App->renderer3D->aabbs.push_back(aabb);
	
}

Component* GameObject::CreateComponent(Component_Type type)
{
	Component* component = nullptr;
	if (HasComponentType(type))
		return component;

	switch (type)
	{
	case Component_Type::Transform:
		component = new ComponentTransform(this, { 0,0,0 }, Quat::identity, { 1,1,1 });
		component->name = "Transform";
		break;
	case Component_Type::Transform2D:
		component = new ComponentTransform2D(this, { 0,0 }, { 0,0 }, { 1,1 });
		component->name = "Transform2D";
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
	case Component_Type::Image:
		component = new ComponentImage(this);
		component->name = "Image";
		break;
	case Component_Type::Button:
		component = new ComponentButton(this);
		component->name = "Button";
		break;
	case Component_Type::Checkbox:
		component = new ComponentCheckbox(this);
		component->name = "Check Box";
		break;
	case Component_Type::Inputbox:
		component = new ComponentInputbox(this);
		component->name = "Input Box";
		break;
	}
	components.push_back(component);
	
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
	if (is_ui) {
		if (game_obj->IsUI()) {
			game_obj->trans2D->SetMatricesWithNewParent(trans2D->GetGlobalMatrix());
		}
		else {
			game_obj->trans->SetMatricesWithNewParent(trans2D->GetGlobalMatrix());
		}

	}
	else {
		if (game_obj->IsUI()) {
			game_obj->trans2D->SetMatricesWithNewParent(trans->GetGlobalMatrix());
		}
		else {
			game_obj->trans->SetMatricesWithNewParent(trans->GetGlobalMatrix());
		}
	}
}

void GameObject::UpdateAABB()
{
	if (!HasComponentType(Component_Type::Mesh)) {
		aabb.SetNegativeInfinity();
		return;
	}

	//obb
	ComponentMesh* mesh = (ComponentMesh*)GetComponent(Component_Type::Mesh);
	if (mesh == nullptr || mesh->resource_mesh == nullptr) {
		aabb.SetNegativeInfinity();
		return;
	}
	obb = mesh->resource_mesh->GetAABB();
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

void GameObject::RemoveResourceModel()
{
	if (resource_model != nullptr) {
		App->resources->FreeResource(resource_model);
	}
	resource_model = nullptr;
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
	
	 JsonArray transArr = obj.AddArray("Components");
	for (std::vector<Component*>::iterator it = components.begin(); it!=components.end(); it++) {
		JsonObj componentObject;
		transArr.AddObject(componentObject);
		componentObject.AddString("name", (*it)->name.c_str());
		(*it)->SaveComponent(componentObject);
	}
	return true;
}


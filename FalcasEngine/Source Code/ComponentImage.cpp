#include "ComponentImage.h"
#include "External Libraries/ImGui/imgui.h"


ComponentImage::ComponentImage(GameObject* owner) : Component(Component_Type::Image, owner, "Image")
{
}


ComponentImage::~ComponentImage()
{

}

void ComponentImage::Update()
{
	/*for (int i = 0; i < App->scene_intro->resources_material_to_delete.size(); i++) {
		if (resource_material == App->scene_intro->resources_material_to_delete[i]) {
			resource_material = nullptr;
			return;
		}
	}*/
}

void ComponentImage::Initialization()
{
}

void ComponentImage::Render()
{
}

bool ComponentImage::SaveComponent(JsonObj& obj)
{
	//obj.AddInt("Resource_ID", resource_material != nullptr ? resource_material->GetID() : 0);
	return true;
}

void ComponentImage::Inspector()
{
	bool falsed = false;

	ImGui::PushID(name.c_str());
	Component::Inspector();


	ImGui::Separator();

	ImGui::PopID();
}

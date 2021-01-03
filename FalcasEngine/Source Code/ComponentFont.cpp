#include "ComponentFont.h"
#include "Application.h"
#include "ModuleResources.h"
#include "ResourceMaterial.h"
#include "ModuleUI.h"
#include "ModuleSceneIntro.h"
#include "ResourceMesh.h"
#include "External Libraries/ImGui/imgui.h"
#include "ComponentTransform2D.h"

void ComponentFont::Load(uint id, std::string characters, uint rows, uint h, uint w, uint rc)
{

	if (id == NULL || characters.c_str() == nullptr || rows == 0)
	{
		LOG("Could not load font");
		return;
	}

	font.texture = (ResourceMaterial*)App->resources->GetResource(id);

	if (font.texture == nullptr)
	{
		LOG("Could not load font at %s with characters '%s'", id, characters);
	}



	font.rows = rows; // rows: rows of characters in the texture
	font.len = 0; // len: length of the table

	font.char_h = h;
	font.char_w = w;
	font.row_chars = rc;
	for (int i = 0; characters[i] == NULL; i++) {
		font.table[i] = characters[i];
	}
}

ComponentFont::ComponentFont(GameObject* owner, ComponentTransform2D* trans) : ComponentUI(Component_Type::Font, owner, "Text"), trans(trans)
{
	Initialization();
}

ComponentFont::~ComponentFont()
{
	if (font.texture != nullptr && !App->resources->isResourcesMapEmpty()) {
		App->resources->FreeResource(font.texture);
	}
	font.texture = nullptr;
}

void ComponentFont::Update()
{
	if (is_clicked_first_frame) {
		OnTriggered(this);
	}
	for (int i = 0; i < App->scene_intro->resources_material_to_delete.size(); i++) {
		if (font.texture == App->scene_intro->resources_material_to_delete[i]) {
			font.texture = nullptr;
		}
	}
}

void ComponentFont::Initialization()
{
	Load(App->UI->mesh_plane_id, "", 0, 0, 0, 0);
}

void ComponentFont::Render()
{
	if (!active)
		return;
	uint len = text.length();
	uint x = 0, y = 0;
	for (int i = 0; i < len; ++i) {
		for (int j = 0; j < 256; ++j) {
			if (text[i] == font.table[j]) {
				RenderChar(font.texture, i);
				break;
			}
			if (j % font.row_chars == 0) { 
				y += font.char_h; 
				x = 0;
			}
			else {
				x += font.char_w;
			}
		}
	}

}

void ComponentFont::RenderChar(ResourceMaterial* c, uint pos)
{
	ComponentTransform2D* transChat = trans;
	transChat->SetPosition({ trans->GetPosition().x + pos * font.char_w,trans->GetPosition().y });
	resource_mesh->Render((float*)&transChat->GetGlobalMatrixTransposed(), nullptr, false, false, false, c);
}

bool ComponentFont::SaveComponent(JsonObj& obj)
{
	return true;
}

void ComponentFont::Inspector()
{
	ImGui::PushID(name.c_str());
	ImGui::Text("gola");
	ImGui::PopID();
}

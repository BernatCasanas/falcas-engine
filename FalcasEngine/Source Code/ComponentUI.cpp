#include "ComponentUI.h"
#include "Application.h"
#include "ModuleResources.h"
#include "ResourceMesh.h"
#include "ModuleUI.h"
#include "GameObject.h"
#include "ComponentTransform2D.h"
#include "ModuleCentralEditor.h"
#include "External Libraries/ImGui/imgui.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ComponentButton.h"

ComponentUI::ComponentUI(Component_Type type, GameObject* owner, std::string name) : Component(type, owner, name)
{
	resource_mesh = (ResourceMesh*)App->resources->RequestResource(App->UI->mesh_plane_id);
	App->UI->UIs.push_back(owner);
	id_vector_uis = App->UI->UIs.size() - 1;
	_font = std::shared_ptr<GLFont>(new GLFont("Assets/Fonts/arial.ttf"));
	_label = std::unique_ptr<FTLabel>(new FTLabel(
		_font,
		_text.c_str(),
		0,
		0,
		App->window->width,
		App->window->height,
		curTex
	));
}

ComponentUI::~ComponentUI()
{
	if (resource_mesh != nullptr && !App->resources->isResourcesMapEmpty()) {
		App->resources->FreeResource(resource_mesh);
	}
	resource_mesh = nullptr;
	App->UI->DeleteUI(id_vector_uis);
}

void ComponentUI::Render()
{
}

void ComponentUI::Update()
{
	if (is_draggable && is_clicked) {
		ComponentTransform2D* trans = (ComponentTransform2D*)owner->components[0];
		float2 pos=trans->GetPosition();
		pos.x += App->input->GetMouseXMotion();
		pos.y -= App->input->GetMouseYMotion();
		trans->SetPosition(pos);
	}
}

void ComponentUI::OnTriggered(ComponentUI* component_ui)
{
	listener = App->central_editor;
	if (listener == nullptr)
		return;

	listener->OnTriggered(component_ui);
}


bool ComponentUI::CheckMouseHovering()
{
	if (!is_focusable || App->input->GetMouseButton(1)==KEY_REPEAT)
		return false;

	UpdateTriangles();
	
	float scene_x, scene_y, scene_width, scene_height;
	App->scene_intro->GetSceneDimensions(scene_x, scene_y, scene_width, scene_height);
	float3 mouse = { 0,0,0 };
	mouse.x = App->input->GetMouseX() - scene_x;
	mouse.y = App->input->GetMouseY() - scene_y;
	if (triangle1.Contains(mouse) || triangle2.Contains(mouse)) {
		is_mouse_hover = true;
	}
	else {
		is_mouse_hover = false;
	}
	return is_mouse_hover;
}

void ComponentUI::UpdateTriangles()
{
	float3 min = resource_mesh->GetAABB().minPoint;
	float3 max = resource_mesh->GetAABB().maxPoint;
	float2 min_p = { min.x,min.y };
	float2 max_p = { max.x,max.y };
	float2 third_p = { min.x, max.y };
	float2 third_p_second_tri = { max.x,min.y };
	((ComponentTransform2D*)owner->GetComponent(Component_Type::Transform2D))->GetTrianglePoints(min_p, max_p, third_p, third_p_second_tri);
	min = { min_p.x,min_p.y,0 };
	max = { max_p.x,max_p.y,0 };
	float3 third = { third_p.x,third_p.y,0 };
	triangle1 = { min,max,third };
	third = { third_p_second_tri.x,third_p_second_tri.y,0 };
	triangle2 = { min,max,third };

}

void ComponentUI::IsClicked(bool clicked_with_mouse)
{
	is_clicked_with_enter = false;
	if (!is_focusable || (!is_mouse_hover && clicked_with_mouse) || (!clicked_with_mouse && !is_focused)) {
		is_focused = false;
		App->UI->focus_ui_id = -1;
		return;
	}
	if (!is_clicked) {
		is_clicked_first_frame = true;
		if (!clicked_with_mouse) {
			is_clicked_with_enter = true;
		}
	}
	else {
		is_clicked_first_frame = false;
	}
	is_clicked = true;
	is_focused = true;
	App->UI->focus_ui_id = id_vector_uis;
}

void ComponentUI::StoppedClicking(bool clicked_with_mouse)
{
	is_clicked = false;
	is_clicked_first_frame = false;
	if (clicked_with_mouse && type != Component_Type::Inputbox) {
		is_focused = false;
	}
}

void ComponentUI::PrintText(std::string text, std::string size, ImVec4 color, ComponentTransform2D* pos)
{
	float scene_x, scene_y, scene_width, scene_height;
	App->scene_intro->GetSceneDimensions(scene_x, scene_y, scene_width, scene_height);
	_label.get()->Initialize(curTex);
	_label.get()->setFont(_font);
	if (size == "") size = "0";
	_label.get()->setPixelSize(std::stoi(size));
	_label.get()->setText((char*)text.c_str());
	_label.get()->setAlignment(FTLabel::FontFlags::Indented);
	float2 _pos = { pos->GetPosition().x , -pos->GetPosition().y };
	_label.get()->setColor(color.x, color.y, color.z, color.w);
	_label.get()->setWindowSize(scene_width, scene_height);
	_pos.x += scene_x;
	_pos.y += (scene_height/2)-scene_y;
	_label.get()->setPosition(_pos.x, _pos.y);

	_label.get()->render();
	_label.get()->Finish();
}

void ComponentUI::Inspector()
{
	ImGui::Checkbox("Draggable", &is_draggable);
}

void ComponentUI::SaveText(JsonObj& obj)
{
	obj.AddString("Text", _text.c_str());
	obj.AddString("Size", _size.c_str());

}

void ComponentUI::LoadText(JsonObj& obj)
{
	_text = obj.GetString("Text");
	_size = obj.GetString("Size");
	_color = ImVec4(obj.GetInt("r"), obj.GetInt("g"), obj.GetInt("b"), obj.GetInt("a"));
}

void ComponentUI::TextInspector()
{
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Text: ");
	ImGui::SameLine();
	ImGui::InputText("##text", &_text, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll);
	ImGui::SameLine();
	ImGui::InputText("##text1", &_size, ImGuiInputTextFlags_EnterReturnsTrue);
	ImGui::Separator();

	ImGui::ColorPicker4("Color Picker", (float*)&_color);
}

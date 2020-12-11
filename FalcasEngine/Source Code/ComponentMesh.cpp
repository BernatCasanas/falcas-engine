#include "Application.h"
#include "ModuleCentralEditor.h"
#include "ComponentTransform.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "Importer.h"
#include "External Libraries/ImGui/imgui.h"
#include "ResourceMesh.h"

ComponentMesh::ComponentMesh(GameObject* owner) :Component(Component_Type::Mesh, owner, "Mesh")
{
}

ComponentMesh::~ComponentMesh()
{
	
}

void ComponentMesh::Update()
{
	if(!owner->IsCulled())
		Render();
}




void ComponentMesh::Initialization()
{
	owner->UpdateAABB();
}

void ComponentMesh::Render()
{
	if (resource_mesh == nullptr)
		return;

	ComponentMaterial* mat = nullptr;
	if(owner->HasComponentType(Component_Type::Material))
		mat = (ComponentMaterial*)owner->GetComponent(Component_Type::Material);

	ComponentTransform* trans = (ComponentTransform*)owner->GetComponent(Component_Type::Transform);
	resource_mesh->Render((float*)&trans->GetGlobalMatrixTransposed(), mat, show_normals_v, length_normals, show_normals_f);
}

bool ComponentMesh::SaveComponent(JsonObj& obj)
{
	//obj.AddString("Path", resource_mesh->libraryPath.c_str());
	obj.AddString("AssetPath", resource_mesh->full_file_name.c_str());
	obj.AddInt("MaterialIndex", resource_mesh->materialIndex);
	obj.AddInt("MeshNumber", this->meshNumber);
	obj.AddInt("UUID", GetUUID());
	return true;
}


void ComponentMesh::Inspector()
{
	bool falsed = false;
	float null = 0;

	ImGui::PushID(name.c_str());
	Component::Inspector();
	
	ImGui::Separator();
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("File: ");
	
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), resource_mesh != nullptr ? resource_mesh->file_name.c_str() : "None");
	
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip(resource_mesh != nullptr ? resource_mesh->full_file_name.c_str() : "");
	}
	
	ImGui::Separator();
	
	ImGui::Columns(2, "", true);
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Show Normals");
	
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Indices: ");
	
	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(resource_mesh != nullptr ? std::to_string(resource_mesh->num_indices).c_str() : "0");
	
	ImGui::NextColumn();
	ImGui::Checkbox("Per Triangle", (active && owner->active) ?&show_normals_v : &falsed);
	
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Vertices: ");
	
	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(resource_mesh != nullptr ? std::to_string(resource_mesh->num_vertices / 3).c_str() : "0");
	
	ImGui::NextColumn();
	ImGui::Checkbox("Per Face", (active && owner->active) ? &show_normals_f:&falsed);
	
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Normals: ");
	
	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(resource_mesh != nullptr ? std::to_string(resource_mesh->num_normals / 3).c_str() : "0");
	
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Length Normals");
	
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Faces: ");
	
	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(resource_mesh != nullptr ? std::to_string(resource_mesh->num_indices / 3).c_str() : "0");
	
	ImGui::NextColumn();
	ImGui::PushItemWidth(120);
	ImGui::DragFloat("##1", (active && owner->active) ? &length_normals:&null, 0.1f);
	ImGui::PopItemWidth();
	
	ImGui::Columns(1, "", false);
	ImGui::Separator();

	ImGui::PopID();
}


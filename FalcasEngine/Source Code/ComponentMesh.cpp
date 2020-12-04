#include "External Libraries/Glew/include/glew.h"
#include "Application.h"
#include "ModuleCentralEditor.h"
#include "ComponentTransform.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "Importer.h"
#include "External Libraries/ImGui/imgui.h"

ComponentMesh::ComponentMesh(GameObject* owner) :Component(Component_Type::Mesh, owner, "Mesh")
{
}

ComponentMesh::~ComponentMesh()
{
	if (vertices != nullptr)
		delete[] vertices;
	if (indices != nullptr)
		delete[] indices;
	if (normals != nullptr)
		delete[] normals;
	if (texCoords != nullptr)
		delete[]texCoords;
	glDeleteBuffers(1, &id_indices);
	glDeleteBuffers(1, &id_vertices);
	glDeleteBuffers(1, &id_normals);
	glDeleteBuffers(1, &id_texCoords);
}

void ComponentMesh::Update()
{
	if(!owner->IsCulled())
		Render();
}




void ComponentMesh::Initialization()
{
	glGenBuffers(1, (GLuint*)&(id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&(id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * num_indices, indices, GL_STATIC_DRAW);
	if (num_textureCoords > 0 && grid == false) {
		glGenBuffers(1, (GLuint*)&(id_normals));
		glBindBuffer(GL_NORMAL_ARRAY, id_normals);
		glBufferData(GL_NORMAL_ARRAY, sizeof(float) * num_vertices, normals, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*)&(id_texCoords));
		glBindBuffer(GL_ARRAY_BUFFER, id_texCoords);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_textureCoords * 2, texCoords, GL_STATIC_DRAW);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_NORMAL_ARRAY, 0);

	SetAABB();
}

void ComponentMesh::Render()
{
	if (id_indices > 0 && id_vertices > 0) {
		ComponentTransform* trans = (ComponentTransform*)owner->GetComponent(Component_Type::Transform);
		glPushMatrix();
		glMultMatrixf((float*)&trans->GetGlobalMatrixTransposed());
		//vertices
		glEnableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);


		//normals
		if (num_normals > 0) {
			glEnableClientState(GL_NORMAL_ARRAY);
			glBindBuffer(GL_NORMAL_ARRAY, id_normals);
			glNormalPointer(GL_FLOAT, 0, NULL);
		}

		if (num_textureCoords > 0 &&owner->GetComponent(Component_Type::Material)) {
			ComponentMaterial* mat = (ComponentMaterial*)owner->GetComponent(Component_Type::Material);
			if (mat->active == true) {
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glBindBuffer(GL_ARRAY_BUFFER, id_texCoords);
				glTexCoordPointer(2, GL_FLOAT, 0, NULL);
				if (!mat->show_default_tex) {
					glBindTexture(GL_TEXTURE_2D, mat->texture_id);
				}
				else {
					glBindTexture(GL_TEXTURE_2D, mat->defaultTex);
				}
			}

		}

		//indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);


		//drawing indices
		if (grid == false) {
			glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);
		}
		else
			glDrawElements(GL_LINES, num_indices, GL_UNSIGNED_INT, NULL);



		//drawing normals
		if ((App->central_editor->normals_v || show_normals_v) && normals != nullptr) {
			if (id_normals == -1)
				return;

			glBegin(GL_LINES);
			for (int i = 0; i < num_vertices; i += 3)
			{
				glColor3f(0.0f, 1.f, 1.f);
				glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
				glVertex3f(vertices[i] + (-normals[i] * length_normals), vertices[i + 1] + (-normals[i + 1] * length_normals), vertices[i + 2] + (-normals[i + 2]) * length_normals);
			}

			glColor3f(1.0f, 1.0f, 1.0f);
			glEnd();
		}
		if ((App->central_editor->normals_f || show_normals_f) && normals != nullptr) {
			if (id_normals == -1)
				return;
			glBegin(GL_LINES);
			for (size_t i = 0; i < num_vertices; i += 3)
			{
				glColor3f(1.0f, 0.f, 1.f);
				float vx = (vertices[i] + vertices[i + 3] + vertices[i + 6]) / 3;
				float vy = (vertices[i + 1] + vertices[i + 4] + vertices[i + 7]) / 3;
				float vz = (vertices[i + 2] + vertices[i + 5] + vertices[i + 8]) / 3;

				float nx = (normals[i] + normals[i + 3] + normals[i + 6]) / 3;
				float ny = (normals[i + 1] + normals[i + 4] + normals[i + 7]) / 3;
				float nz = (normals[i + 2] + normals[i + 5] + normals[i + 8]) / 3;

				glVertex3f(vx, vy, vz);

				glVertex3f(vx + (normals[i] * length_normals),
					vy + (normals[i + 1] * length_normals),
					vz + (normals[i + 2]) * length_normals);
			}
			glColor3f(1.0f, 1.0f, 1.0f);
			glEnd();
		}


		//cleaning stuff

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_NORMAL_ARRAY, 0);
		glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		if (num_normals > 0) {
			glDisableClientState(GL_NORMAL_ARRAY);
		}
		if (num_textureCoords > 0 && grid == false) {
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_ELEMENT_ARRAY_BUFFER);
		glPopMatrix();
	}
}

bool ComponentMesh::SaveComponent(JsonObj& obj)
{
	obj.AddString("Path", this->libraryPath.c_str());
	obj.AddString("AssetPath", this->full_file_name.c_str());
	obj.AddInt("MaterialIndex", this->materialIndex);
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
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), file_name.c_str());
	
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip(full_file_name.c_str());
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
	ImGui::Text(std::to_string(num_indices).c_str());
	
	ImGui::NextColumn();
	ImGui::Checkbox("Per Triangle", (active && owner->active) ?&show_normals_v : &falsed);
	
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Vertices: ");
	
	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(std::to_string(num_vertices / 3).c_str());
	
	ImGui::NextColumn();
	ImGui::Checkbox("Per Face", (active && owner->active) ? &show_normals_f:&falsed);
	
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Normals: ");
	
	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(std::to_string(num_normals / 3).c_str());
	
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Length Normals");
	
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Faces: ");
	
	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(std::to_string(num_indices / 3).c_str());
	
	ImGui::NextColumn();
	ImGui::PushItemWidth(120);
	ImGui::DragFloat("##1", (active && owner->active) ? &length_normals:&null, 0.1f);
	ImGui::PopItemWidth();
	
	ImGui::Columns(1, "", false);
	ImGui::Separator();

	ImGui::PopID();
}

AABB ComponentMesh::GetAABB() const
{
	return aabb;
}

void ComponentMesh::SetAABB()
{
	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)vertices, num_vertices/3);
	owner->UpdateAABB();
}

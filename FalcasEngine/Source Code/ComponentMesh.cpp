#pragma once
#include "External Libraries/Glew/include/glew.h"
#include "Application.h"
#include "ModuleCentralEditor.h"
#include <vector>
#include "ModuleSceneIntro.h"
#include "GameObject.h"
#include "External Libraries/Assimp/Assimp/include/cimport.h"
#include "External Libraries/Assimp/Assimp/include/postprocess.h"
#include "External Libraries/Assimp/Assimp/include/scene.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"


ComponentMesh::ComponentMesh(GameObject* owner) :Component(Component_Type::Mesh, owner)
{
	grid = false;
	vertices = nullptr;
	indices = nullptr;
	normals = nullptr;
	show_normals = false;
	length_normals = 1;
	id_indices = id_vertices = num_indices = num_vertices = id_normals = num_normals = 0;
	name = "Mesh";
	full_file_name = "";
	file_name = "";
	loading = false;
}

ComponentMesh::~ComponentMesh()
{
	if (vertices != nullptr)
		delete[] vertices;
	if (indices != nullptr)
		delete[] indices;
	if (normals != nullptr)
		delete[] normals;
	glDeleteBuffers(1, &id_indices);
	glDeleteBuffers(1, &id_vertices);
	glDeleteBuffers(1, &id_normals);
}

void ComponentMesh::SetFileName(std::string file)
{
	full_file_name = file;
	int pos = -1;
	pos = full_file_name.find_last_of('\\');
	if (pos == -1)
		pos = full_file_name.find_last_of('/');
	file_name = full_file_name.substr(pos + 1);
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
}

void ComponentMesh::Render()
{
	if (id_indices > 0 && id_vertices > 0) {
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

		if (num_textureCoords > 0 && grid == false) {
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
			//glBindTexture(GL_TEXTURE_2D, 2);
			glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);
		}
		else
			glDrawElements(GL_LINES, num_indices, GL_UNSIGNED_INT, NULL);



		//drawing normals
		if ((App->central_editor->normals || show_normals) && normals != nullptr) {

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



		//cleaning stuff
		if (num_normals > 0) {
			glBindBuffer(GL_NORMAL_ARRAY, 0);
			glDisableClientState(GL_NORMAL_ARRAY);
		}
		if (num_textureCoords > 0 && grid == false) {
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_ELEMENT_ARRAY_BUFFER);
	}
}

void ComponentMesh::Inspector()
{
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
	ImGui::Checkbox("Per Triangle", &show_normals);
	
	ImGui::NextColumn();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Vertices: ");
	
	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(std::to_string(num_vertices / 3).c_str());
	
	ImGui::NextColumn();
	ImGui::Checkbox("Per Face", &show_normals);
	
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
	ImGui::DragFloat("##1", &length_normals, 0.1f);
	ImGui::PopItemWidth();
	
	ImGui::Columns(1, "", false);
	ImGui::Separator();

	ImGui::PopID();
}

void ComponentMesh::LoadMesh(float3 position, const char* file, std::string name, ComponentMaterial* mat)
{
	loading = true;
	GameObject* m=nullptr;
	ComponentMesh* m_mesh=nullptr;
	ComponentMaterial* m_material=nullptr;
	bool multimesh = false;
	const aiScene* scene = nullptr;
	scene = aiImportFile(file, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		if (scene->mNumMeshes > 1) {
			multimesh = true;
		}
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			if (multimesh) {
				m = App->filesystem->CreateGameObject(name, owner);
				m_mesh = (ComponentMesh*)m->CreateComponent(Component_Type::Mesh);
				
				m_material=(ComponentMaterial*)m->CreateComponent(Component_Type::Material);
			}
			else {
				m = owner;
				m_mesh = (ComponentMesh*)m->GetComponent(Component_Type::Mesh);

				m_material = (ComponentMaterial*)m->GetComponent(Component_Type::Material);
			}
			*m_material = *mat;
			m_mesh->SetFileName(file);
			aiMesh* ai_mesh = scene->mMeshes[i];
			m_mesh->num_vertices = ai_mesh->mNumVertices*3;
			m_mesh->vertices = new float[m_mesh->num_vertices];
			memcpy(m_mesh->vertices, ai_mesh->mVertices, sizeof(float) * m_mesh->num_vertices);
			LOG("Loading FBX correctly");
			LOG("New mesh with %d vertices", m_mesh->num_vertices/3);

			if (ai_mesh->HasFaces())
			{
				m_mesh->num_indices = ai_mesh->mNumFaces * 3;
				m_mesh->indices = new uint[m_mesh->num_indices];
				for (uint j = 0; j < ai_mesh->mNumFaces; ++j)
				{
					if (ai_mesh->mFaces[j].mNumIndices != 3) {
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else {
						memcpy(&m_mesh->indices[j * 3], ai_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
					}

				}
				LOG("New mesh with %d index", m_mesh->num_indices);
			}
			m_mesh->num_normals = m_mesh->num_vertices;
			m_mesh->normals = new float[ai_mesh->mNumVertices * 3];
			for (int x = 0, y = 0; x < ai_mesh->mNumVertices; x++, y += 3) {
				if (ai_mesh->HasNormals())
				{
					m_mesh->normals[y] = ai_mesh->mNormals[x].x;
					m_mesh->normals[y + 1] = ai_mesh->mNormals[x].y;
					m_mesh->normals[y + 2] = ai_mesh->mNormals[x].z;
				}
			}

			if (ai_mesh->HasTextureCoords(0)) {
				m_mesh->num_textureCoords = ai_mesh->mNumVertices;
				m_mesh->texCoords = new float[m_mesh->num_textureCoords * 2];
				for (uint i = 0, j = 0; i < m_mesh->num_textureCoords; i++, j += 2) {
					m_mesh->texCoords[j] = ai_mesh->mTextureCoords[0][i].x;
					m_mesh->texCoords[j + 1] = ai_mesh->mTextureCoords[0][i].y;
				}
			}

			m_mesh->Initialization();
		}
		aiReleaseImport(scene);
	}
	else {
		const char* error = aiGetErrorString();
		LOG("Error loading FBX: %s", error)
	}
	loading = false;
}

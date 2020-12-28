#include "ResourceMesh.h"
#include "External Libraries/Glew/include/glew.h"
#include "ComponentMaterial.h"
#include "ResourceMaterial.h"
#include "Application.h"
#include "FileSystem.h"
#include "ModuleCentralEditor.h"


ResourceMesh::ResourceMesh(uint ID, Resource_Type type, std::string assets_file): Resource(ID,type,assets_file), full_file_name(assets_file)
{
	file_name = App->filesystem->GetFileName(full_file_name, false);
}

bool ResourceMesh::CleanUp()
{
	if (vertices != nullptr) {
		delete[] vertices;
		vertices = nullptr;
	}
	if (indices != nullptr) {
		delete[] indices;
		indices = nullptr;
	}
	if (normals != nullptr) {
		delete[] normals;
		normals = nullptr;
	}
	if (texCoords != nullptr) {
		delete[]texCoords;
		texCoords = nullptr;
	}
	glDeleteBuffers(1, &id_indices);
	glDeleteBuffers(1, &id_vertices);
	glDeleteBuffers(1, &id_normals);
	glDeleteBuffers(1, &id_texCoords);
	return true;
}

void ResourceMesh::Initialize()
{
	glGenBuffers(1, (GLuint*)&(id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&(id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * num_indices, indices, GL_STATIC_DRAW);
	if (num_textureCoords > 0) {
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

void ResourceMesh::Render(float* transform, ComponentMaterial* mat, bool show_normals_v, float length_normals, bool show_normals_f, ResourceMaterial* res_mat)
{
	if (id_indices > 0 && id_vertices > 0) {
		glPushMatrix();
		glMultMatrixf(transform);
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

		if (num_textureCoords > 0 && ((mat != nullptr && mat->active == true) || res_mat != nullptr)) {
			
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, id_texCoords);
			glTexCoordPointer(2, GL_FLOAT, 0, NULL);
			if (res_mat != nullptr || (!mat->show_default_tex && mat->resource_material != nullptr)) {
				if (mat == nullptr) {
					glBindTexture(GL_TEXTURE_2D, res_mat->texture_id);
				}
				else {
					glBindTexture(GL_TEXTURE_2D, mat->resource_material->texture_id);
				}
			}
			else {
				glBindTexture(GL_TEXTURE_2D, mat->defaultTex);
			}
		}

		//indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);


		//drawing indices
			glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);
	

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
		if (num_textureCoords > 0) {
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_ELEMENT_ARRAY_BUFFER);
		glPopMatrix();
	}
}

AABB ResourceMesh::GetAABB() const
{
	return aabb;
}

void ResourceMesh::SetAABB()
{
	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)vertices, num_vertices / 3);
}

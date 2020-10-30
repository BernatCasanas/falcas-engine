#pragma once
#include "External Libraries/Glew/include/glew.h"
#include "Application.h"
#include "ModuleCentralEditor.h"
#include "External Libraries/MathGeoLib/include/Math/MathFunc.h"
#include <vector>
#include "ModuleSceneIntro.h"
#include "GameObject.h"
#include "External Libraries/Assimp/Assimp/include/cimport.h"
#include "External Libraries/Assimp/Assimp/include/postprocess.h"
#include "External Libraries/Assimp/Assimp/include/scene.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"

#define M_PI       3.14159265358979323846

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
	parent = owner;
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
	int pos = full_file_name.find_last_of('\\');
	file_name = full_file_name.substr(pos + 1);
}


void ComponentMesh::CreateCube(float3 position, float3 size)
{
	std::vector<float> vertices2;
	std::vector<float> indices2;

	vertices2 = { 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 0.f, 1.f, 1.f, 1.f };
	num_vertices = vertices2.size();
	vertices = new float[num_vertices];
	for (int i = 0; i < num_vertices; i++) {
		if (size.x > 0) {
			vertices2[i] *= size.x;
		}
		int j = i + 1;
		if (j % 3 == 0) {
			vertices2[i] += position.z;
		}
		else if ((j + 1) % 3 == 0) {
			vertices2[i] += position.y;
		}
		else {
			vertices2[i] += position.x;
		}
		vertices[i] = vertices2[i];
	}


	indices2 = { 1, 5, 3, 5, 7, 3, 4, 2, 6, 4, 0, 2, 5, 4, 7, 4, 6, 7, 1, 3, 0, 0, 3, 2, 3, 7, 2, 7, 6, 2, 1, 0, 5, 5, 0, 4 };
	num_indices = indices2.size();
	indices = new uint[num_indices];
	for (int i = 0; i < num_indices; i++) {
		indices[i] = indices2[i];
	}
	this->Initialization();
}

void ComponentMesh::CreatePrism(float3 position, float3 size)
{
	std::vector<float> vertices2;
	std::vector<float> indices2;

	vertices2 = { 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 0.f, 1.f, 1.f, 1.f };
	num_vertices = vertices2.size();
	vertices = new float[num_vertices];
	for (int i = 0; i < num_vertices; i++) {

		int j = i + 1;
		if (j % 3 == 0) {
			if (size.x > 0) {
				vertices2[i] *= size.x;
			}
			vertices2[i] += position.z;
		}
		else if ((j + 1) % 3 == 0) {
			if (size.y > 0) {
				vertices2[i] *= size.y;
			}
			vertices2[i] += position.y;
		}
		else {
			if (size.z > 0) {
				vertices2[i] *= size.z;
			}
			vertices2[i] += position.x;
		}
		vertices[i] = vertices2[i];

	}


	indices2 = { 1, 5, 3, 5, 7, 3, 4, 2, 6, 4, 0, 2, 5, 4, 7, 4, 6, 7, 1, 3, 0, 0, 3, 2, 3, 7, 2, 7, 6, 2, 1, 0, 5, 5, 0, 4 };
	num_indices = indices2.size();
	indices = new uint[num_indices];
	for (int i = 0; i < num_indices; i++) {
		indices[i] = indices2[i];
	}
	this->Initialization();
}

void ComponentMesh::CreateTriPyramid(float3 position, float3 size)
{
	std::vector<float> vertices2;
	std::vector<float> indices2;
	float z_side = Cos(DegToRad(30));
	vertices2 = { 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.5f, 0.f, z_side, 0.5f,0.816,z_side / 2 };
	num_vertices = vertices2.size();
	vertices = new float[num_vertices];
	for (int i = 0; i < num_vertices; i++) {
		if (size.x > 0) {
			vertices2[i] *= size.x;
		}
		int j = i + 1;
		if (j % 3 == 0) {
			vertices2[i] += position.z;
		}
		else if ((j + 1) % 3 == 0) {
			vertices2[i] += position.y;
		}
		else {
			vertices2[i] += position.x;
		}
		vertices[i] = vertices2[i];
	}


	indices2 = { 1, 3, 2, 0, 3, 1, 2, 3, 0, 2, 0, 1 };
	num_indices = indices2.size();
	indices = new uint[num_indices];
	for (int i = 0; i < num_indices; i++) {
		indices[i] = indices2[i];
	}

	this->Initialization();
}

void ComponentMesh::CreateSqrPyramid(float3 position, float3 size)
{
	std::vector<float> vertices2;
	std::vector<float> indices2;

	vertices2 = { 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.5f,1.f,0.5f };
	num_vertices = vertices2.size();
	vertices = new float[num_vertices];
	for (int i = 0; i < num_vertices; i++) {
		int j = i + 1;
		if (size.x > 0) {
			if ((j + 1) % 3 != 0) {
				vertices2[i] *= size.x;
			}
		}
		if (size.y > 0) {
			if ((j + 1) % 3 == 0) {
				vertices2[i] *= size.y;
			}
		}
		if (j % 3 == 0) {
			vertices2[i] += position.z;
		}
		else if ((j + 1) % 3 == 0) {
			vertices2[i] += position.y;
		}
		else {
			vertices2[i] += position.x;
		}
		vertices[i] = vertices2[i];
	}


	indices2 = { 3, 4, 1, 2, 4, 3, 0, 4, 2, 1, 4, 0, 3, 1, 0, 0, 2, 3 };
	num_indices = indices2.size();
	indices = new uint[num_indices];
	for (int i = 0; i < num_indices; i++) {
		indices[i] = indices2[i];
	}
	this->Initialization();
}

void ComponentMesh::CreateRectPyramid(float3 position, float3 size)
{
	std::vector<float> vertices2;
	std::vector<float> indices2;
	vertices2 = { 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.5f,1.f,0.5f };
	num_vertices = vertices2.size();
	vertices = new float[num_vertices];
	for (int i = 0; i < num_vertices; i++) {
		int j = i + 1;
		if (j % 3 == 0) {
			if (size.x > 1) {
				vertices2[i] *= size.x;
			}
			vertices2[i] += position.z;
		}
		else if ((j + 1) % 3 == 0) {
			if (size.y > 1) {
				vertices2[i] *= size.y;
			}
			vertices2[i] += position.y;
		}
		else {
			if (size.z > 1) {
				vertices2[i] *= size.z;
			}
			vertices2[i] += position.x;
		}
		vertices[i] = vertices2[i];
	}


	indices2 = { 3, 4, 1, 2, 4, 3, 0, 4, 2, 1, 4, 0, 3, 1, 0, 0, 2, 3 };
	num_indices = indices2.size();
	indices = new uint[num_indices];
	for (int i = 0; i < num_indices; i++) {
		indices[i] = indices2[i];
	}
	this->Initialization();
}

void ComponentMesh::CreateSphere(float3 position, float3 size, uint rings, uint sectors)
{
	std::vector<float> vertices2;
	std::vector<float> indices2;
	float sectorCount = rings;
	float stackCount = sectors;
	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / size.x;    // vertex normal
	float s, t;                                     // vertex texCoord

	float sectorStep = 2 * M_PI / sectorCount;
	float stackStep = M_PI / stackCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = size.x * cosf(stackAngle);             // r * cos(u)
		z = size.x * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			vertices2.push_back(x + position.x);
			vertices2.push_back(y + position.y);
			vertices2.push_back(z + position.z);
		}
	}
	num_vertices = vertices2.size();
	vertices = new float[num_vertices];
	for (int i = 0; i < num_vertices; i++) {
		vertices[i] = vertices2[i];
	}
	int k1, k2;
	for (int i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);     // beginning of current stack
		k2 = k1 + sectorCount + 1;      // beginning of next stack

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices2.push_back(k1);
				indices2.push_back(k2);
				indices2.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (stackCount - 1))
			{
				indices2.push_back(k1 + 1);
				indices2.push_back(k2);
				indices2.push_back(k2 + 1);
			}
		}
	}
	num_indices = indices2.size();
	indices = new uint[num_indices];
	for (int i = 0; i < num_indices; i++) {
		indices[i] = indices2[i];
	}
	this->Initialization();
}

void ComponentMesh::CreateCilinder(float3 position, float3 size, uint rings, uint sectors)
{
	std::vector<float> vertices2;
	std::vector<float> indices2;
	vertices = nullptr;
	float angle = 360 / sectors;
	float midlength = (float)size.y / ((float)rings + 1);
	vertices2.push_back(0 + position.x);
	vertices2.push_back(0 + position.y);
	vertices2.push_back(0 + position.z);
	if (rings > 0 && sectors > 0) {
		for (int j = 0; j <= rings + 1; j++) {
			float length = midlength * j;
			for (int i = 0; i < sectors; i++) {
				float theta = angle * i;
				theta = DegToRad(theta);
				vertices2.push_back((size.x * cos(theta)) + position.x);
				vertices2.push_back(length + position.y);
				vertices2.push_back((size.x * sin(theta)) + position.z);

			}
		}
	}
	vertices2.push_back(0 + position.x);
	vertices2.push_back(size.y + position.y);
	vertices2.push_back(0 + position.z);

	num_vertices = vertices2.size();
	vertices = new float[num_vertices];
	for (int i = 0; i < num_vertices; i++) {
		vertices[i] = vertices2[i];
	}
	indices = nullptr;

	for (int j = 0; j <= rings + 2; j++) {
		for (int i = 1; i <= sectors; i++) {
			int i2 = i + sectors * (j - 1);
			if (j == 0) {
				indices2.push_back(0);
				indices2.push_back(i);
				if (i != sectors)
					indices2.push_back(i + 1);
				else
					indices2.push_back(1);
			}
			else if (j - 2 != rings) {
				if (i2 / j != sectors)
					indices2.push_back(i2 + 1);
				else
					indices2.push_back(i2 - sectors + 1);
				indices2.push_back(i2);
				indices2.push_back(i2 + sectors);
				if (i2 / j != sectors)
					indices2.push_back(i2 + 1);
				else
					indices2.push_back(i2 - sectors + 1);
				indices2.push_back(i2 + sectors);
				if (i2 / j != sectors)
					indices2.push_back(i2 + sectors + 1);
				else
					indices2.push_back(i2 + 1);
			}
			else {
				indices2.push_back((num_vertices / 3) - 1);
				if (i2 / j != sectors)
					indices2.push_back(i2 + 1);
				else
					indices2.push_back(i2 - sectors + 1);
				indices2.push_back(i2);
			}
		}
	}





	num_indices = indices2.size();
	indices = new uint[num_indices];
	for (int i = 0; i < num_indices; i++) {
		indices[i] = indices2[i];
	}
	this->Initialization();
}

void ComponentMesh::CreateCone(float3 position, float3 size, uint sectors)
{

	uint rings = 1;
	float smallest_radius = (float)size.x / 2;
	float radius2 = size.x;
	std::vector<float> vertices2;
	std::vector<float> indices2;
	vertices = nullptr;
	float angle = 360 / sectors;
	float midlength = (float)size.y / ((float)rings + 1);
	vertices2.push_back(0 + position.x);
	vertices2.push_back(0 + position.y);
	vertices2.push_back(0 + position.z);
	if (rings > 0 && sectors > 0) {
		for (int j = 0; j <= rings + 1; j++) {
			radius2 = size.x - (smallest_radius * (j / rings));
			float length = midlength * j;
			for (int i = 0; i < sectors; i++) {
				float theta = angle * i;
				theta = DegToRad(theta);
				vertices2.push_back((radius2 * cos(theta)) + position.x);
				vertices2.push_back(length + position.y);
				vertices2.push_back((radius2 * sin(theta)) + position.z);

			}
		}
	}
	vertices2.push_back(0 + position.x);
	vertices2.push_back(size.y + position.y);
	vertices2.push_back(0 + position.z);

	num_vertices = vertices2.size();
	vertices = new float[num_vertices];
	for (int i = 0; i < num_vertices; i++) {
		vertices[i] = vertices2[i];
	}
	indices = nullptr;

	for (int j = 0; j <= rings + 2; j++) {
		for (int i = 1; i <= sectors; i++) {
			int i2 = i + sectors * (j - 1);
			if (j == 0) {
				indices2.push_back(0);
				indices2.push_back(i);
				if (i != sectors)
					indices2.push_back(i + 1);
				else
					indices2.push_back(1);
			}
			else if (j - 2 != rings) {
				if (i2 / j != sectors)
					indices2.push_back(i2 + 1);
				else
					indices2.push_back(i2 - sectors + 1);
				indices2.push_back(i2);
				indices2.push_back(i2 + sectors);
				if (i2 / j != sectors)
					indices2.push_back(i2 + 1);
				else
					indices2.push_back(i2 - sectors + 1);
				indices2.push_back(i2 + sectors);
				if (i2 / j != sectors)
					indices2.push_back(i2 + sectors + 1);
				else
					indices2.push_back(i2 + 1);
			}
			else {
				indices2.push_back((num_vertices / 3) - 1);
				if (i2 / j != sectors)
					indices2.push_back(i2 + 1);
				else
					indices2.push_back(i2 - sectors + 1);
				indices2.push_back(i2);
			}
		}
	}





	num_indices = indices2.size();
	indices = new uint[num_indices];
	for (int i = 0; i < num_indices; i++) {
		indices[i] = indices2[i];
	}
	this->Initialization();
}

void ComponentMesh::CreatePlane(float3 position, float3 size)
{
	size.y = 1;
	std::vector<float> vertices2;
	std::vector<float> indices2;

	vertices2 = { 0.f,0.f,0.f,1.f,0.f,0.f,1.f,0.f,1.f,0.f,0.f,1.f };
	num_vertices = vertices2.size();
	vertices = new float[num_vertices];
	for (int i = 0; i < num_vertices; i++) {
		if (size.x > 0) {
			vertices2[i] *= size.x;
		}
		int j = i + 1;
		if (j % 3 == 0) {
			vertices2[i] += position.z;
		}
		else if ((j + 1) % 3 == 0) {
			vertices2[i] += position.y;
		}
		else {
			vertices2[i] += position.x;
		}
		vertices[i] = vertices2[i];
	}
	indices2 = { 1,0,2,3,2,0 };
	num_indices = indices2.size();
	indices = new uint[num_indices];
	for (int i = 0; i < num_indices; i++) {
		indices[i] = indices2[i];
	}
	this->Initialization();
}

void ComponentMesh::CreateGrid(float3 position, float3 size)
{
	grid = true;
	size.y = 1;
	vertices = nullptr;
	int size2 = size.x;
	std::vector<float> vertices2;


	for (int z = -size.x; z <= size2; z++) {
		for (int x = -size.x; x <= size2; x += size.x + size.x) {

			vertices2.push_back(x);
			vertices2.push_back(0);
			vertices2.push_back(z);
		}
	}

	for (int x = -size.x; x <= size2; x++) {
		for (int z = -size.x; z <= size2; z += size.x + size.x) {
			vertices2.push_back(x);
			vertices2.push_back(0);
			vertices2.push_back(z);
		}
	}
	num_vertices = vertices2.size();
	vertices = new float[num_vertices];
	num_indices = num_vertices;
	indices = new uint[num_indices];
	for (int i = 0; i < num_vertices; i++) {
		vertices[i] = vertices2[i];
		indices[i] = i;
	}
	uint* ind = indices + 2;
	this->Initialization();
}



void ComponentMesh::Initialization()
{
	glGenBuffers(1, (GLuint*)&(id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices, vertices, GL_STATIC_DRAW);
	glGenBuffers(1, (GLuint*)&(id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * num_indices, indices, GL_STATIC_DRAW);
	if (loading) {
		glGenBuffers(1, (GLuint*)&(id_normals));
		glBindBuffer(GL_NORMAL_ARRAY, id_normals);
		glBufferData(GL_NORMAL_ARRAY, sizeof(float) * num_vertices, normals, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*)&(num_textureCoords));
		glBindBuffer(GL_NORMAL_ARRAY, id_texCoords);
		glBufferData(GL_NORMAL_ARRAY, sizeof(float) * num_textureCoords, texCoords, GL_STATIC_DRAW);
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
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_NORMAL_ARRAY, id_normals);
		glNormalPointer(GL_FLOAT, 0, NULL);

		//indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);

		//drawing indices
		if (grid == false)
			glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);
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
			glColor3f(1.0f, 1.0f,1.0f);
			glEnd();



		}
		ComponentMaterial* material = parent->material;
		/*if (material != nullptr) {
			if (material->wantTex) {
				glBindTexture(GL_TEXTURE_2D, material->defaultTex);
			}
			else glBindTexture(GL_TEXTURE_2D, material->texture_id);
		}*/
		//glBindTexture(GL_TEXTURE_2D, material->defaultTex);

		//cleaning stuff
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_NORMAL_ARRAY, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

bool ComponentMesh::Inspector(Gui_Type& type, int& index, std::string& info, bool*& checked, float*& number, bool& same_line, bool& separator_in_column, bool& next_column,
	int& num_columns, float& width, float4& color)
{
	info =  "";
	same_line = separator_in_column = next_column = false;
	num_columns = 1;
	width = 50;
	switch (index)
	{
	case 0:
		Component::Inspector(type, index, info, checked, number, same_line, separator_in_column, next_column, num_columns, width, color);
		break;
	case 1:
	case 5:
		type = Gui_Type::Separator;
		index++;
		break;
	case 2:
	case 7:
	case 8:
	case 9:
	case 11:
	case 12:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
		switch (index)
		{
		case 2:
		case 7:
		case 9:
		case 12:
		case 15:
		case 18:
			switch (index)
			{
			case 2:
				info = "File: ";
				break;
			case 7:
				info = "Show Normals";
				break;
			case 9:
				info = std::to_string(num_indices);
				same_line = true;
				break;
			case 12:
				info = std::to_string(num_vertices);
				same_line = true;
				break;
			case 15:
				info = std::to_string(num_normals);
				same_line = true;
				break;
			case 18:
				info = std::to_string(num_indices / 3);
				same_line = true;
				break;
			}
			break;
		case 8:
		case 11:
		case 14:
		case 16:
		case 17:
			switch (index)
			{
			case 8:
				info = "Indices: ";
				break;
			case 11:
				info = "Vertices: ";
				break;
			case 14:
				info = "Normals: ";
				break;
			case 16:
				info = "Length Normals";
				break;
			case 17:
				info = "Faces:";
				break;
			}
			next_column = true;
			break;
		}
		type = Gui_Type::Text;
		index++;
		break;
	case 3:
		info = file_name.c_str();
		color = { 1.0f,1.0f,0.0f,1.0f };
		same_line = true;
		type = Gui_Type::TextColored;
		index++;
		break;
	case 4:
		info = full_file_name.c_str();
		type = Gui_Type::Tooltip;
		index++;
		break;
	case 6:
		num_columns = 2;
		separator_in_column = true;
	case 20:
		type = Gui_Type::Column;
		index++;
		break;
	case 10:
	case 13:
		switch (index)
		{
		case 10:
			checked = &show_normals;
			info = "Per Triangle";
			break;
		case 13:
			info = "Per Face";
			break;
		}
		next_column = true;
		type = Gui_Type::CheckBox;
		index++;
		break;
	case 19:
		number = &length_normals;
		next_column = true;
		type = Gui_Type::DragFloat;
		index++;
		break;
	default:
		return false;
		break;
	}
	return true;
}

void ComponentMesh::LoadMesh(float3 position, const char* file, std::string name)
{
	loading = true;
	GameObject* m;
	ComponentMesh* m_mesh;
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
				m = App->scene_intro->CreateGameObject(name, parent);
				m_mesh = (ComponentMesh*)m->CreateComponent(Component_Type::Mesh);
			}
			else m_mesh = this;
			if (scene->mMeshes[i] == NULL) continue;
			m_mesh->SetFileName(file);
			aiMesh* ai_mesh = scene->mMeshes[i];
			m_mesh->num_vertices = ai_mesh->mNumVertices;
			m_mesh->vertices = new float[m_mesh->num_vertices * 3];
			memcpy(m_mesh->vertices, ai_mesh->mVertices, sizeof(float) * m_mesh->num_vertices);
			LOG("Loading FBX correctly");
			LOG("New mesh with %d vertices", m_mesh->num_vertices);

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
					//normal copying
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

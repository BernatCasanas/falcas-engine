#include "Shape.h"
#include "External Libraries/Glew/include/glew.h"
#include "External Libraries/MathGeoLib/include/Math/MathFunc.h"
#include <vector>

#define M_PI       3.14159265358979323846

GameObject::GameObject(Shape shape, float3 position, std::string name)
{
	this->position = position;
	this->shape = shape;
	this->name = name;
	id_indices = id_vertices = num_indices = 0;
	vertices = nullptr;
	indices = nullptr;
}

GameObject::~GameObject()
{
	if(vertices!=nullptr)
		delete[] vertices;
	if(indices!=nullptr)
		delete[] indices;
	glDeleteBuffers(1,&id_indices);
	glDeleteBuffers(1, &id_vertices);
}

void GameObject::Render(bool triangles)
{
	if (id_indices > 0 && id_vertices > 0) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
		if(triangles==true)
			glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);
		else
			glDrawElements(GL_LINES, num_indices, GL_UNSIGNED_INT, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

void GameObject::Initialization()
{
	glGenBuffers(1, (GLuint*)&(id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices, vertices, GL_STATIC_DRAW);
	glGenBuffers(1, (GLuint*)&(id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * num_indices, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Grid::Grid(Shape shape, float3 position, std::string name, uint size) : GameObject(shape, position, name)
{
	this->size = size;
	vertices=nullptr;
	int size2 = size;
	std::vector<float> vertices2;
	
	
	for (int z = -size; z <= size2; z++) {
		for (int x = -size; x <= size2; x += size + size) {

			vertices2.push_back(x);
			vertices2.push_back(0);
			vertices2.push_back(z);
		}
	}
	
	for (int x = -size; x <= size2; x++) {
		for (int z = -size; z <= size2; z += size + size) {
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

SolidPlane::SolidPlane(Shape shape, float3 position, std::string name, uint size) : GameObject(shape, position, name)
{
	this->size = size;
	std::vector<float> vertices2;
	std::vector<float> indices2;

	vertices2 = { 0.f,0.f,0.f,1.f,0.f,0.f,1.f,0.f,1.f,0.f,0.f,1.f };
	num_vertices = vertices2.size();
	vertices = new float[num_vertices];
	for (int i = 0; i < num_vertices; i++) {
		if (size > 0) {
			vertices2[i] *= size;
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



Cube::Cube(Shape shape, float3 position, std::string name, uint size) :GameObject(shape, position, name)
{
	this->size = size;
	std::vector<float> vertices2;
	std::vector<float> indices2;

	vertices2 = { 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 0.f, 1.f, 1.f, 1.f };
	num_vertices = vertices2.size();
	vertices = new float[num_vertices];
	for (int i = 0; i < num_vertices; i++) {
		if (size > 0) {
			vertices2[i] *= size;
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


RectangularPrism::RectangularPrism(Shape shape, float3 position, std::string name, uint size_length, uint size_height, uint size_width) :GameObject(shape, position, name)
{
	this->size_length = size_length;
	this->size_height = size_height;
	this->size_width = size_width;
	std::vector<float> vertices2;
	std::vector<float> indices2;

	vertices2 = { 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 0.f, 1.f, 1.f, 1.f };
	num_vertices = vertices2.size();
	vertices = new float[num_vertices];
	for (int i = 0; i < num_vertices; i++) {
		
		int j = i + 1;
		if (j % 3 == 0) {
			if (size_length > 0) {
				vertices2[i] *= size_length;
			}
			vertices2[i] += position.z;
		}
		else if ((j + 1) % 3 == 0) {
			if (size_height > 0) {
				vertices2[i] *= size_height;
			}
			vertices2[i] += position.y;
		}
		else {
			if (size_width > 0) {
				vertices2[i] *= size_width;
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
TriangularPyramid::TriangularPyramid(Shape shape, float3 position, std::string name, uint size) :GameObject(shape, position, name)
{
	this->size = size;
	std::vector<float> vertices2;
	std::vector<float> indices2;
	float z_side = Cos(DegToRad(30));
	vertices2 = { 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.5f, 0.f, z_side, 0.5f,0.816,z_side/2 };
	num_vertices = vertices2.size();
	vertices = new float[num_vertices];
	for (int i = 0; i < num_vertices; i++) {
		if (size > 0) {
			vertices2[i] *= size;
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


SquarePyramid::SquarePyramid(Shape shape, float3 position, std::string name, uint height, uint size_base) :GameObject(shape, position, name)
{
	this->height = height;
	this->size_base = size_base;
	std::vector<float> vertices2;
	std::vector<float> indices2;

	vertices2 = { 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.5f,1.f,0.5f };
	num_vertices = vertices2.size();
	vertices = new float[num_vertices];
	for (int i = 0; i < num_vertices; i++) {
		int j = i + 1;
		if (size_base > 0) {
			if ((j + 1) % 3 != 0) {
				vertices2[i] *= size_base;
			}
		}
		if (height > 0) {
			if ((j + 1) % 3 == 0) {
				vertices2[i] *= height;
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

RectangularPyramid::RectangularPyramid(Shape shape, float3 position, std::string name, uint height, uint size_length, uint size_width) :GameObject(shape, position, name)
{
	this->height = height;
	this->size_length = size_length;
	this->size_width = size_width;
	std::vector<float> vertices2;
	std::vector<float> indices2;
	vertices2 = { 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.5f,1.f,0.5f };
	num_vertices = vertices2.size();
	vertices = new float[num_vertices];
	for (int i = 0; i < num_vertices; i++) {
		int j = i + 1;
		if (j % 3 == 0) {
			if (size_length > 1) {
				vertices2[i] *= size_length;
			}
			vertices2[i] += position.z;
		}
		else if ((j + 1) % 3 == 0) {
			if (height > 1) {
				vertices2[i] *= height;
			}
			vertices2[i] += position.y;
		}
		else {
			if (size_width > 1) {
				vertices2[i] *= size_width;
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

SolidSphere::SolidSphere(Shape shape, float3 position, std::string name, uint radius, uint rings, uint sectors) :GameObject(shape, position, name)
{
	std::vector<float> vertices2;
	std::vector<float> indices2;
	this->radius = radius;
	this->rings = rings;
	this->sectors = sectors;
	float sectorCount = rings;
	float stackCount = sectors;
	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
	float s, t;                                     // vertex texCoord

	float sectorStep = 2 * M_PI / sectorCount;
	float stackStep = M_PI / stackCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

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

Cilinder::Cilinder(Shape shape, float3 position, std::string name, uint radius, uint height, uint rings, uint sectors) :GameObject(shape, position, name)
{
	this->radius = radius;
	this->rings = rings;
	this->sectors = sectors;
	this->height = height;
	std::vector<float> vertices2;
	std::vector<float> indices2;
	vertices=nullptr;
	float angle = 360 / sectors;
	float midlength = (float)height / ((float)rings+1);
	vertices2.push_back(0 + position.x);
	vertices2.push_back(0 + position.y);
	vertices2.push_back(0 + position.z);
	if (rings > 0 && sectors > 0) {
		for (int j = 0; j <= rings+1; j++) {
			float length = midlength * j;
			for (int i = 0; i < sectors; i++) {
				float theta = angle * i;
				theta = DegToRad(theta);
				vertices2.push_back((radius * cos(theta))+position.x);
				vertices2.push_back(length + position.y);
				vertices2.push_back((radius* sin(theta)) + position.z);

			}
		}
	}
	vertices2.push_back(0 + position.x);
	vertices2.push_back(height + position.y);
	vertices2.push_back(0 + position.z);
	
	num_vertices = vertices2.size();
	vertices = new float[num_vertices];
	for (int i = 0; i < num_vertices; i++) {
		vertices[i] = vertices2[i];
	}
	indices=nullptr;

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
				indices2.push_back((num_vertices/3)-1);
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

SolidCone::SolidCone(Shape shape, float3 position, std::string name, uint radius, uint height,  uint sectors) : GameObject(shape, position, name)
{
	this->radius = radius;
	uint rings = 1;
	this->sectors = sectors;
	this->height = height;
	float smallest_radius = (float)radius / 2;
	float radius2 = radius;
	std::vector<float> vertices2;
	std::vector<float> indices2;
	vertices = nullptr;
	float angle = 360 / sectors;
	float midlength = (float)height / ((float)rings + 1);
	vertices2.push_back(0 + position.x);
	vertices2.push_back(0 + position.y);
	vertices2.push_back(0 + position.z);
	if (rings > 0 && sectors > 0) {
		for (int j = 0; j <= rings + 1; j++) {
			radius2 = radius - (smallest_radius * (j / rings));
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
	vertices2.push_back(height + position.y);
	vertices2.push_back(0 + position.z);

	num_vertices = vertices2.size();
	vertices = new float[num_vertices];
	for (int i = 0; i < num_vertices; i++) {
		vertices[i] = vertices2[i];
	}
	indices=nullptr;

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

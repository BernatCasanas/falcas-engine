#include "Shape.h"
#include "External Libraries/Glew/include/glew.h"
#include "External Libraries/MathGeoLib/include/Math/MathFunc.h"
#include <vector>

#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923

Shape::Shape(Shapes shape, float3 position)
{
	this->position = position;
	this->shape = shape;
	id_indices = id_vertices = num_indices = 0;
}

void Shape::Render()
{
	if (id_indices > 0 && id_vertices > 0) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
		glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void Shape::Initialization()
{
	glGenBuffers(1, (GLuint*)&(id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glGenBuffers(1, (GLuint*)&(id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


Cube::Cube(Shapes shape, float3 position, uint size) :Shape(shape, position)
{
	this->size = size;
	

	vertices = { 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 0.f, 1.f, 1.f, 1.f };
	num_vertices = vertices.size();
	for (int i = 0; i < num_vertices; i++) {
		if (size > 0) {
			vertices[i] *= size;
		}
		int j = i + 1;
		if (j % 3 == 0) {
			vertices[i] += position.z;
		}
		else if ((j + 1) % 3 == 0) {
			vertices[i] += position.y;
		}
		else {
			vertices[i] += position.x;
		}
	}


	indices = { 1, 5, 3, 5, 7, 3, 4, 2, 6, 4, 0, 2, 5, 4, 7, 4, 6, 7, 1, 3, 0, 0, 3, 2, 3, 7, 2, 7, 6, 2, 1, 0, 5, 5, 0, 4 };
	num_indices = indices.size();

	this->Initialization();
	
}


RectangularPrism::RectangularPrism(Shapes shape, float3 position, uint size_length, uint size_height, uint size_width) :Shape(shape, position)
{
	this->size_length = size_length;
	this->size_height = size_height;
	this->size_width = size_width;


	vertices = { 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 0.f, 1.f, 1.f, 1.f };
	num_vertices = vertices.size();
	for (int i = 0; i < num_vertices; i++) {
		
		int j = i + 1;
		if (j % 3 == 0) {
			if (size_length > 0) {
				vertices[i] *= size_length;
			}
			vertices[i] += position.z;
		}
		else if ((j + 1) % 3 == 0) {
			if (size_height > 0) {
				vertices[i] *= size_height;
			}
			vertices[i] += position.y;
		}
		else {
			if (size_width > 0) {
				vertices[i] *= size_width;
			}
			vertices[i] += position.x;
		}
	}


	indices = { 1, 5, 3, 5, 7, 3, 4, 2, 6, 4, 0, 2, 5, 4, 7, 4, 6, 7, 1, 3, 0, 0, 3, 2, 3, 7, 2, 7, 6, 2, 1, 0, 5, 5, 0, 4 };
	num_indices = indices.size();

	this->Initialization();

}
TriangularPyramid::TriangularPyramid(Shapes shape, float3 position, uint size) :Shape(shape, position)
{
	this->size = size;

	float z_side = Cos(DegToRad(30));
	vertices = { 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.5f, 0.f, z_side, 0.5f,0.816,z_side/2 };
	num_vertices = vertices.size();
	for (int i = 0; i < num_vertices; i++) {
		if (size > 0) {
			vertices[i] *= size;
		}
		int j = i + 1;
		if (j % 3 == 0) {
			vertices[i] += position.z;
		}
		else if ((j + 1) % 3 == 0) {
			vertices[i] += position.y;
		}
		else {
			vertices[i] += position.x;
		}
	}


	indices = { 1, 3, 2, 0, 3, 1, 2, 3, 0, 2, 0, 1 };
	num_indices = indices.size();


	this->Initialization();
}


SquarePyramid::SquarePyramid(Shapes shape, float3 position, uint height, uint size_base) :Shape(shape, position)
{
	this->height = height;
	this->size_base = size_base;


	vertices = { 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.5f,1.f,0.5f };
	num_vertices = vertices.size();
	for (int i = 0; i < num_vertices; i++) {
		int j = i + 1;
		if (size_base > 0) {
			if ((j + 1) % 3 != 0) {
				vertices[i] *= size_base;
			}
		}
		if (height > 0) {
			if ((j + 1) % 3 == 0) {
				vertices[i] *= height;
			}
		}
		if (j % 3 == 0) {
			vertices[i] += position.z;
		}
		else if ((j + 1) % 3 == 0) {
			vertices[i] += position.y;
		}
		else {
			vertices[i] += position.x;
		}
	}


	indices = { 3, 4, 1, 2, 4, 3, 0, 4, 2, 1, 4, 0, 3, 1, 0, 0, 2, 3 };
	num_indices = indices.size();


	this->Initialization();
}

RectangularPyramid::RectangularPyramid(Shapes shape, float3 position, uint height, uint size_length, uint size_width) :Shape(shape, position)
{
	this->height = height;
	this->size_length = size_length;
	this->size_width = size_width;

	vertices = { 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.5f,1.f,0.5f };
	num_vertices = vertices.size();
	for (int i = 0; i < num_vertices; i++) {
		int j = i + 1;
		if (j % 3 == 0) {
			if (size_length > 1) {
				vertices[i] *= size_length;
			}
			vertices[i] += position.z;
		}
		else if ((j + 1) % 3 == 0) {
			if (height > 1) {
				vertices[i] *= height;
			}
			vertices[i] += position.y;
		}
		else {
			if (size_width > 1) {
				vertices[i] *= size_width;
			}
			vertices[i] += position.x;
		}
	}


	indices = { 3, 4, 1, 2, 4, 3, 0, 4, 2, 1, 4, 0, 3, 1, 0, 0, 2, 3 };
	num_indices = indices.size();
	this->Initialization();

}

SolidSphere::SolidSphere(Shapes shape, float3 position, uint radius, uint rings, uint sectors) :Shape(shape, position)
{
	this->radius = radius;
	this->rings = rings;
	this->sectors = sectors;
	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	int r, s;

	vertices.resize(rings * sectors * 3);
	std::vector<float>::iterator v = vertices.begin();

	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
		float const y = sin(-M_PI_2 + M_PI * r * R);
		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
		float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

		*v++ = x * radius;
		*v++ = y * radius;
		*v++ = z * radius;
	}

	indices.resize(rings * sectors * 4);
	std::vector<int>::iterator i = indices.begin();
	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
		*i++ = r * sectors + s;
		*i++ = r * sectors + (s + 1);
		*i++ = (r + 1) * sectors + (s + 1);
		*i++ = (r + 1) * sectors + s;
	}
	this->Initialization();
}

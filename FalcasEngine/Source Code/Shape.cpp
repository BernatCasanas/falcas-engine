#include "Shape.h"
#include "External Libraries/Glew/include/glew.h"
#include <vector>


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
	}
}


Cube::Cube(Shapes shape, float3 position, uint size) :Shape(shape, position)
{
	this->size = size;


	float vertices[] = { 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 0.f, 1.f, 1.f, 1.f };
	uint num_vertices = sizeof(vertices) / sizeof(float);
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


	int indices[] = { 1, 5, 3, 5, 7, 3, 4, 2, 6, 4, 0, 2, 5, 4, 7, 4, 6, 7, 1, 3, 0, 0, 3, 2, 3, 7, 2, 7, 6, 2, 1, 0, 5, 5, 0, 4 };
	num_indices = sizeof(indices) / sizeof(int);

	glGenBuffers(1, (GLuint*)&(id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenBuffers(1, (GLuint*)&(id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
}


RectangularPrism::RectangularPrism(Shapes shape, float3 position, uint size_length, uint size_height, uint size_width) :Shape(shape, position)
{
	this->size_length = size_length;
	this->size_height = size_height;
	this->size_width = size_width;


	float vertices[] = { 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 0.f, 1.f, 1.f, 1.f };
	uint num_vertices = sizeof(vertices) / sizeof(float);
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


	int indices[] = { 1, 5, 3, 5, 7, 3, 4, 2, 6, 4, 0, 2, 5, 4, 7, 4, 6, 7, 1, 3, 0, 0, 3, 2, 3, 7, 2, 7, 6, 2, 1, 0, 5, 5, 0, 4 };
	num_indices = sizeof(indices) / sizeof(int);

	glGenBuffers(1, (GLuint*)&(id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenBuffers(1, (GLuint*)&(id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}


SquarePyramid::SquarePyramid(Shapes shape, float3 position, uint height, uint size_base) :Shape(shape, position)
{
	this->height = height;
	this->size_base = size_base;


	float vertices[] = { 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.5f,1.f,0.5f };
	uint num_vertices = sizeof(vertices) / sizeof(float);
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


	int indices[] = { 3, 4, 1, 2, 4, 3, 0, 4, 2, 1, 4, 0, 3, 1, 0, 0, 2, 3 };
	num_indices = sizeof(indices) / sizeof(int);


	glGenBuffers(1, (GLuint*)&(id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenBuffers(1, (GLuint*)&(id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

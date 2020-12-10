#include "External Libraries/Glew/include/glew.h"
#include "Shape.h"


Grid::Grid(uint num_vertices, uint num_indices, uint* indices, float* vertices): num_indices(num_indices) {

	glGenBuffers(1, (GLuint*)&(id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&(id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * num_indices, indices, GL_STATIC_DRAW);
}

Grid::~Grid()
{
	glDeleteBuffers(1, &id_indices);
	glDeleteBuffers(1, &id_vertices);
}

void Grid::DrawGrid()
{
	
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glDrawElements(GL_LINES, num_indices, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_ELEMENT_ARRAY_BUFFER);

}

Grid* CreateGrid()
{
	uint num_vertices;
	uint num_indices;
	uint* indices;
	float* vertices;
	float3 size = { 500,500,500 };
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
	Grid* grid = new Grid(num_vertices, num_indices, indices, vertices);
	return grid;
}

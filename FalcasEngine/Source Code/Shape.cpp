
#include "Shape.h"
#include <vector>
#include "External Libraries/MathGeoLib/include/Math/MathFunc.h"

#define M_PI       3.14159265358979323846


void CreateCube(uint& num_vertices, uint& num_indices, uint*& indices, float*& vertices, float3 position, float3 size)
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
}

void CreatePrism(uint& num_vertices, uint& num_indices, uint*& indices, float*& vertices, float3 position, float3 size)
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
}

void CreateTriPyramid(uint& num_vertices, uint& num_indices, uint*& indices, float*& vertices, float3 position, float3 size)
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

}

void CreateSqrPyramid(uint& num_vertices, uint& num_indices, uint*& indices, float*& vertices, float3 position, float3 size)
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
}

void CreateRectPyramid(uint& num_vertices, uint& num_indices, uint*& indices, float*& vertices, float3 position, float3 size)
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
}

void CreateSphere(uint& num_vertices, uint& num_indices, uint*& indices, float*& vertices, float3 position, float3 size, uint rings, uint sectors)
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
}

void CreateCilinder(uint& num_vertices, uint& num_indices, uint*& indices, float*& vertices, float3 position, float3 size, uint rings, uint sectors)
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
}

void CreateCone(uint& num_vertices, uint& num_indices, uint*& indices, float*& vertices, float3 position, float3 size, uint sectors)
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
}

void CreatePlane(uint& num_vertices, uint& num_indices, uint*& indices, float*& vertices, float3 position, float3 size)
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
}

void CreateGrid(uint& num_vertices, uint& num_indices, uint*& indices, float*& vertices, float3 position, float3 size)
{
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
}

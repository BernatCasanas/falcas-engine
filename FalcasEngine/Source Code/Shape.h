#include "Globals.h"
#include "External Libraries/MathGeoLib/include/Math/float3.h"
#include <vector>


enum class Shapes {
	Unknown=-1,
	Cube,
	RectangularPrism,
	TriangularPyramid,
	SquarePyramid,
	RectangularPyramid,
};

class Shape {
public:
	Shape(Shapes shape, float3 position);
	virtual ~Shape(){};
	void Render();
	void Initialization();
protected:
	uint id_vertices;
	uint id_indices;
	uint num_indices;
	uint num_vertices;
	std::vector<float> vertices;
	std::vector<int> indices;
	float3 position;
private:
	Shapes shape;
};

class Cube : public Shape {
public:
	Cube(Shapes shape, float3 position, uint size);
	virtual ~Cube() {};
private:
	uint size;
};
class RectangularPrism : public Shape {
public:
	RectangularPrism(Shapes shape, float3 position, uint size_length, uint size_height, uint size_width);
	virtual ~RectangularPrism() {};
private:
	uint size_length;
	uint size_height;
	uint size_width;
};
class TriangularPyramid : public Shape {
public:
	TriangularPyramid(Shapes shape, float3 position, uint size);
	virtual ~TriangularPyramid() {};
private:
	uint size;
};
class SquarePyramid : public Shape {
public:
	SquarePyramid(Shapes shape, float3 position, uint height, uint size_base);
	virtual ~SquarePyramid() {};
private:
	uint height;
	uint size_base;
};
class RectangularPyramid : public Shape {
public:
	RectangularPyramid(Shapes shape, float3 position, uint height, uint size_length, uint size_width);
	virtual ~RectangularPyramid() {};
private:
	uint height;
	uint size_length;
	uint size_width;
};
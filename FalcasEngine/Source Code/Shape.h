#include "Globals.h"
#include "External Libraries/MathGeoLib/include/Math/float3.h"
#include <vector>


enum class Shape {
	Unknown=-1,
	Empty,
	Grid,
	SolidPlane,
	Cube,
	RectangularPrism,
	TriangularPyramid,
	SquarePyramid,
	RectangularPyramid,
	SolidSphere,
	Cilinder,
	SolidCone
};

class GameObject {
public:
	GameObject(Shape shape, float3 position, std::string name);
	~GameObject();
	void Render(bool triangles = true);
	void Initialization();
	float3 GetPosition() { return position; }
	void GetPosition(float& x, float& y, float& z) { x = position.x, y = position.y, z = position.z; }
	std::string GetName() { return name; }
protected:
	uint id_vertices;
	uint id_indices;
	uint num_indices;
	uint num_vertices;
	std::vector<float> vertices;
	std::vector<int> indices;
	float3 position;
	std::string name;
private:
	Shape shape;
};

class Grid :public GameObject {
public:
	Grid(Shape shape, float3 position, std::string name, uint size);
private:
	uint size;
};

class SolidPlane :public GameObject {
public:
	SolidPlane(Shape shape, float3 position, std::string name, uint size);
private:
	uint size;
};

class Cube : public GameObject {
public:
	Cube(Shape shape, float3 position, std::string name, uint size);
private:
	uint size;
};
class RectangularPrism : public GameObject {
public:
	RectangularPrism(Shape shape, float3 position, std::string name, uint size_length, uint size_height, uint size_width);
private:
	uint size_length;
	uint size_height;
	uint size_width;
};
class TriangularPyramid : public GameObject {
public:
	TriangularPyramid(Shape shape, float3 position, std::string name, uint size);
private:
	uint size;
};
class SquarePyramid : public GameObject {
public:
	SquarePyramid(Shape shape, float3 position, std::string name, uint height, uint size_base);
private:
	uint height;
	uint size_base;
};
class RectangularPyramid : public GameObject {
public:
	RectangularPyramid(Shape shape, float3 position, std::string name, uint height, uint size_length, uint size_width);
private:
	uint height;
	uint size_length;
	uint size_width;
};
class SolidSphere : public GameObject {
public:
	SolidSphere(Shape shape, float3 position, std::string name, uint radius, uint rings, uint sectors);
private:
	uint radius;
	uint rings;
	uint sectors;
};
class Cilinder : public GameObject {
public:
	Cilinder(Shape shape, float3 position, std::string name, uint radius, uint height, uint rings, uint sectors);
private:
	uint height;
	uint radius;
	uint rings;
	uint sectors;
};
class SolidCone : public GameObject {
public:
	SolidCone(Shape shape, float3 position, std::string name, uint radius, uint height, uint sectors);
private:
	uint height;
	uint radius;
	uint sectors;
};

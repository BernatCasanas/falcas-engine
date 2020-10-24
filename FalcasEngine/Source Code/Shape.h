#include "Globals.h"
#include "External Libraries/MathGeoLib/include/Math/float3.h"
#include <vector>
#include "GameObject.h"


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
	SolidCone,
	Mesh,
};

class GameObjectTemporal {
public:
	GameObjectTemporal(Shape shape, float3 position, std::string name);
	inline GameObjectTemporal::GameObjectTemporal(std::string name, GameObjectTemporal* parent, bool active)
	{
	}
	GameObjectTemporal() {};
	~GameObjectTemporal();
	void Render(bool triangles = true);
	void Initialization();
	float3 GetPosition() { return position; }
	void GetPosition(float& x, float& y, float& z) { x = position.x, y = position.y, z = position.z; }
	std::string GetName() { return name; }
public:
	uint id_vertices;
	uint id_indices;
	uint num_indices;
	uint num_vertices;
	float* vertices;
	uint* indices;
	float3 position;
	std::string name;
private:
	Shape shape;
};

class Grid :public GameObjectTemporal {
public:
	Grid(Shape shape, float3 position, std::string name, uint size);
private:
	uint size;
};

class SolidPlane :public GameObjectTemporal {
public:
	SolidPlane(Shape shape, float3 position, std::string name, uint size);
private:
	uint size;
};

class Cube : public GameObjectTemporal {
public:
	Cube(Shape shape, float3 position, std::string name, uint size);
private:
	uint size;
};
class RectangularPrism : public GameObjectTemporal {
public:
	RectangularPrism(Shape shape, float3 position, std::string name, uint size_length, uint size_height, uint size_width);
private:
	uint size_length;
	uint size_height;
	uint size_width;
};
class TriangularPyramid : public GameObjectTemporal {
public:
	TriangularPyramid(Shape shape, float3 position, std::string name, uint size);
private:
	uint size;
};
class SquarePyramid : public GameObjectTemporal {
public:
	SquarePyramid(Shape shape, float3 position, std::string name, uint height, uint size_base);
private:
	uint height;
	uint size_base;
};
class RectangularPyramid : public GameObjectTemporal {
public:
	RectangularPyramid(Shape shape, float3 position, std::string name, uint height, uint size_length, uint size_width);
private:
	uint height;
	uint size_length;
	uint size_width;
};
class SolidSphere : public GameObjectTemporal {
public:
	SolidSphere(Shape shape, float3 position, std::string name, uint radius, uint rings, uint sectors);
private:
	uint radius;
	uint rings;
	uint sectors;
};
class Cilinder : public GameObjectTemporal {
public:
	Cilinder(Shape shape, float3 position, std::string name, uint radius, uint height, uint rings, uint sectors);
private:
	uint height;
	uint radius;
	uint rings;
	uint sectors;
};
class SolidCone : public GameObjectTemporal {
public:
	SolidCone(Shape shape, float3 position, std::string name, uint radius, uint height, uint sectors);
private:
	uint height;
	uint radius;
	uint sectors;
};

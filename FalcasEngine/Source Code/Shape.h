#include "Globals.h"
#include "External Libraries/MathGeoLib/include/Math/float3.h"

enum class Shapes {
	Unknown=-1,
	Cube,
};

class Shape {
public:
	Shape(Shapes shape, float3 position);
	virtual ~Shape(){};
	void Render();
protected:
	uint id_vertices;
	uint id_indices;
	uint num_indices;
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
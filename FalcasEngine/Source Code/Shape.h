#pragma once
#include "External Libraries/MathGeoLib/include/Math/float3.h"
#include "Globals.h"

class Grid {
public:

	Grid(uint num_vertices, uint num_indices, uint* indices, float* vertices);
	~Grid();
	void DrawGrid();
private:
	uint id_vertices = 0;
	uint id_indices = 0;
	uint num_indices = 0;
};
Grid* CreateGrid();
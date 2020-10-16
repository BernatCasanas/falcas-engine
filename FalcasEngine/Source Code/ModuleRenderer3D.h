#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "External Libraries/SDL/include/SDL_video.h"

#define MAX_LIGHTS 8

class Cube;
class SquarePyramid;
class RectangularPrism;
class TriangularPyramid;
class RectangularPyramid;
class SolidSphere;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	uint DrawCube(uint);
	uint CreateIndices(uint);
	void DrawCubeDirectMode();
	void DrawCubeGLDrawArrays();
	void DrawCubeGLDrawElements();


public:
	Cube* cube;
	SquarePyramid* square_pyramid;
	RectangularPrism* rectangular_prism;
	TriangularPyramid* triangular_pyramid;
	RectangularPyramid* rectangular_pyramid;
	SolidSphere* solid_sphere;
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
};
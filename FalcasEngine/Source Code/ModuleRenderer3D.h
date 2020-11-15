#pragma once
#include "Module.h"
#include "Light.h"
#include "External Libraries/SDL/include/SDL_video.h"
#include "External Libraries/Glew/include/glew.h"
#include "External Libraries/MathGeoLib/include/Geometry/AABB.h"
#include "External Libraries/MathGeoLib/include/Math/float3x3.h"


#define MAX_LIGHTS 8
class ComponentCamera;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void DrawAABB(AABB aabb);
	void DrawFrustum(Frustum frustum);

	void OnResize(int width, int height);



public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	GLuint frameBuffer;
	GLuint texColorBuffer;
	GLuint rboDepthStencil;
	std::vector<AABB> aabbs;
	ComponentCamera* camera_culling = nullptr;
	ComponentCamera* camera = nullptr;



	bool temporal_change_of_camera = false;
};
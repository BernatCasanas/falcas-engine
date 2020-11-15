#pragma once
#include "Module.h"
#include "glmath.h"
#include "External Libraries/MathGeoLib/include/Math/float3.h"

class ComponentCamera;
class ComponentTransform;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();
	void Zooming(float zooming_value);

	ComponentTransform* GetComponentTransform();
	void SetPosition(float3 pos);
	float3 GetPosition();
	Quat GetRotation();

private:

	void CalculateViewMatrix();

public:
	ComponentCamera* camera=nullptr;
	vec3 X, Y, Z, Position, Reference;

private:

	mat4x4 ViewMatrix;
};
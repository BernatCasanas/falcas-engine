#pragma once
#include "Module.h"
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

	void LookAt(const float3&Spot);
	void Zooming(float zooming_value);

	ComponentTransform* GetComponentTransform();
	void SetPosition(float3 pos);
	float3 GetPosition();
	Quat GetRotation();

public:
	ComponentCamera* camera=nullptr;

private:
	float3 Reference;
};
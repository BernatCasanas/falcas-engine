#ifndef _COMPONENT_CAMERA_
#define _COMPONENT_CAMERA_
#pragma once

#include "Component.h"
#include "External Libraries/MathGeoLib/include/Geometry/Frustum.h"

class GameObject;

class ComponentCamera : public Component {
public:
	ComponentCamera(GameObject* owner, float3 pos = { 0,0,0 });
	~ComponentCamera();
public:
	void UpdateFrustum();
	void Inspector();
	void Update();
	bool GetIfIsFrustumCulling() const;


public:
	Frustum frustum;

private:
	bool needed_to_update = false;
	float near_plane_distance=1;
	float far_plane_distance=100;
	float field_of_view_vertical = 1;
	float field_of_view_horizontal = 1;
	bool frustum_culling = false;

};
#endif // !_COMPONENT_CAMERA_

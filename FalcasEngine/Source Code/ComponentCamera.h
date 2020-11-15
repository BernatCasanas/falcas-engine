#ifndef _COMPONENT_CAMERA_
#define _COMPONENT_CAMERA_
#pragma once

#include "Component.h"
#include "External Libraries/MathGeoLib/include/Geometry/Frustum.h"
#include "External Libraries/MathGeoLib/include/Math/float4x4.h"

class GameObject;
class ComponentTransform;

class ComponentCamera : public Component {
public:
	ComponentCamera(GameObject* owner, ComponentTransform* trans);
	~ComponentCamera();
public:
	void UpdateFrustum();
	void Inspector();
	void Update();
	bool GetIfIsFrustumCulling() const;
	float* GetProjectionMatrix() const;
	float* GetViewMatrix() const;
	void ChangeCameraActive();
	void ChangeCullingCamera();
public:
	bool update_projection_matrix = false;
	Frustum frustum;
	bool changed_camera = false;

private:
	bool needed_to_update = false;
	float near_plane_distance = 0.1;
	float far_plane_distance = 1000;
	float field_of_view_vertical = 1;
	float field_of_view_horizontal = 1;
	bool frustum_culling = false;
	float width = 1280;
	float height = 720;
	float4x4 ViewMatrix;
	ComponentTransform* trans;
	bool camera_active = false;
};
#endif // !_COMPONENT_CAMERA_

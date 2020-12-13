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
	bool SaveComponent(JsonObj& obj);
	bool GetIfIsFrustumCulling() const;
	float* GetProjectionMatrix() const;
	float* GetViewMatrix() const;

	float GetFarPlaneDistance();
	float GetNearPlaneDistance();
	float GetHorizFov();
	float GetVerticalFov();

	void SetFarPlaneDistance(float num);
	void SetNearPlaneDistance(float num);
	void SetHorizFov(float num);
	void SetVerticalFov(float num);

	void ShowFrustrum(bool b);

public:
	bool update_projection_matrix = false;
	Frustum frustum;
	bool camera_active = false;
	bool frustum_culling = false;

private:
	bool needed_to_update = false;
	bool show_frustum = true;
	float near_plane_distance = 0.1;
	float far_plane_distance = 1000;
	float field_of_view_vertical = 1;
	float field_of_view_horizontal = 1;
	float width = 1280;
	float height = 720;
	float4x4 ViewMatrix;
	ComponentTransform* trans;
};
#endif // !_COMPONENT_CAMERA_

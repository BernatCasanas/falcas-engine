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
	void CreateFrustum(float3 pos);
	void Inspector();


public:
	Frustum frustum;


};
#endif // !_COMPONENT_CAMERA_#pragma once

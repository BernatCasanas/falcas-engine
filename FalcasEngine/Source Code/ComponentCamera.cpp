#include "ComponentCamera.h"

ComponentCamera::ComponentCamera(GameObject* owner):Component(Component_Type::Camera,owner,"Camera")
{
	frustum;
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::CreateFrustum(float3 pos)
{
	
}

void ComponentCamera::Inspector()
{
}

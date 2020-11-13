#include "ComponentCamera.h"

ComponentCamera::ComponentCamera(GameObject* owner, float3 pos):Component(Component_Type::Camera,owner,"Camera")
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetPos(pos);
	frustum.SetFront({ 0,0,1 });
	frustum.SetUp({ 0,1,0 });

	frustum.SetViewPlaneDistances(1, 100);
	frustum.SetPerspective(1, 1);

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

#pragma once
#include "Component.h"
#include "External Libraries/MathGeoLib/include/Math/float3.h"
#include "External Libraries/MathGeoLib/include/Math/float4x4.h"
#include "External Libraries/MathGeoLib/include/Math/Quat.h"

class GameObject;


class ComponentTransform : public Component {
public:
	ComponentTransform(GameObject* owner, float3 position, Quat rotation, float3 size);
	~ComponentTransform();
	void Update();

	float3 GetPosition()const;
	Quat GetRotation()const;
	float3 GetRotation(bool convert);
	float3 GetSize()const;
	float3 QuaternionToEuler(Quat q);
	Quat EulerToQuaternion(float3 eu);
	float4x4 GetGlobalMatrix() const;
	float4x4 GetGlobalMatrixTransposed() const;


	void SetTransformation(float3 pos, Quat rot, float3 size);
	void SetPosition(float3 pos);
	void SetMatrices();
	void Inspector();

	bool needed_to_update = false;

private:
	float4x4 local_matrix;
	float4x4 global_matrix;
	float4x4 global_matrix_transposed;
	float3 position;
	Quat rotation;
	float3 size;
	float3 euler;
};
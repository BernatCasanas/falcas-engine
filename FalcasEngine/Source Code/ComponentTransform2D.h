#pragma once
#include "Component.h"
#include "External Libraries/MathGeoLib/include/Math/float2.h"
#include "External Libraries/MathGeoLib/include/Math/float4x4.h"
#include "External Libraries/MathGeoLib/include/Math/Quat.h"
#include "Json.h"

class GameObject;


class ComponentTransform2D : public Component {
public:
	ComponentTransform2D(GameObject* owner, float2 position, float2 rotation, float2 size);
	~ComponentTransform2D();
	void Update();

	float2 GetPosition()const;
	float2 GetRotation()const;
	float2 GetSize()const;
	float4x4 GetGlobalMatrix() const;
	float4x4 GetGlobalMatrixTransposed() const;

	bool SaveComponent(JsonObj& obj);
	float3 QuaternionToEuler(Quat q);
	Quat EulerToQuaternion(float3 eu);

	void SetTransformation(float2 pos, float2 rot, float2 size);
	void SetPosition(float2 pos);
	void SetRotation(float2 rot);
	void SetSize(float2 size);
	void SetMatrices();
	void SetMatricesWithNewParent(float4x4 parent_global_matrix);
	void Inspector();
	bool needed_to_update = false;
	bool needed_to_update_only_children = false;


private:
	float4x4 local_matrix;
	float4x4 global_matrix;
	float4x4 global_matrix2;
	float4x4 global_matrix_transposed;
	float2 position;
	float2 pivot_position;
	float2 size;
	float2 rotation;
};
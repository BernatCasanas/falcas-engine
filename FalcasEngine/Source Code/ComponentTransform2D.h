#pragma once
#include "Component.h"
#include "External Libraries/MathGeoLib/include/Math/float2.h"
#include "External Libraries/MathGeoLib/include/Math/float4x4.h"
#include "External Libraries/MathGeoLib/include/Math/Quat.h"
#include "Json.h"

class GameObject;


class ComponentTransform2D : public Component {
public:
	ComponentTransform2D(GameObject* owner, float2 position, Quat rotation, float2 size);
	~ComponentTransform2D();
	void Update();

	float2 GetPosition()const;
	float3 GetRotation()const;
	float2 GetSize()const;
	float4x4 GetGlobalMatrix() const;
	float4x4 GetGlobalMatrixTransposed() const;

	bool SaveComponent(JsonObj& obj);
	float3 QuaternionToEuler(Quat q);
	Quat EulerToQuaternion(float3 eu);

	Quat LookAt(const float3& point);
	
	float2 CalculateMovement(float4x4 matrix, float2 goal);

	void SetTransformation(float3 pos, Quat rot, float2 size, bool guizmo_size = false);
	void SetPosition(float2 pos);
	void SetRotation(Quat rot);
	void SetRotation(float3 rot);
	void SetSize(float2 size);
	void UpdateZ();
	bool UpdateMatrixBillboard();
	void SetMatrices();
	void SetMatricesWithNewParent(float4x4 parent_global_matrix);
	void Inspector();

	void GetTrianglePoints(float2& min_p, float2& max_p, float2& third_p, float2& third_p_second_tri);

	float GetDepth();

public:
	bool needed_to_update = false;
	bool needed_to_update_only_children = false;


private:
	float4x4 local_matrix;
	float4x4 matrix_billboard;
	float4x4 global_matrix;
	float4x4 matrix_pivot;
	float4x4 matrix_parent;
	float4x4 global_matrix_transposed;
	float2 position;
	float z_depth;
	float z_depth_with_layers;
	float2 pivot_position = { 0,0 };
	float2 size;
	float3 rotation;
};
#pragma once
#include "Component.h"
#include "External Libraries/MathGeoLib/include/Math/float3.h"
#include "External Libraries/MathGeoLib/include/Math/float4x4.h"
#include "External Libraries/MathGeoLib/include/Math/Quat.h"
#include "Json.h"

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

	bool SaveComponent(JsonObj& obj);


	void SetTransformation(float3 pos, Quat rot, float3 size);
	void SetPosition(float3 pos);
	void SetPositionFloat(float3 pos);
	void SetRotationFloat(float3 rotation);
	void SetSizeFloat(float3 size);
	void SetRotation(Quat rot);
	void SetMatrices();
	void SetMatricesWithNewParent(float4x4 parent_global_matrix);
	void Inspector();
	void SetPositionToChilds(GameObject* game_object,float3 pos);
	void SetRotationToChilds(GameObject* game_object, float3 rot);
	void SetSizeToChilds(GameObject* game_object, float3 size);
	bool needed_to_update = false;
	bool needed_to_update_only_children = false;


private:
	float4x4 local_matrix;
	float4x4 global_matrix;
	float4x4 global_matrix2;
	float4x4 global_matrix_transposed;
	float3 position;
	Quat rotation;
	float3 size;
	float3 euler;
};
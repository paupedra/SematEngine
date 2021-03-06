#ifndef __COMPONENTTRANSFORM__
#define __COMPONENTTRANSFORM__

#include "Dependecies/MathGeoLib/include/MathGeoLib.h"


class Component;
class JsonNode;

class CTransform : public Component
{
public:
	CTransform(GameObject* owner);
	~CTransform();

	void Update(float dt) override;
	void CleanUp() override;
	void OnPlay() override {};
	void OnStop() override {};

	void Serialize(JsonNode* node)override;
	void Load(JsonNode* node)override;

	void OnSave(JsonNode* node);

	float4x4 GetTransform() const;
	float3 GetPosition()const;
	float3 GetScale()const;
	float4x4 GetGlobalTransform()const;
	float3 GetGlobalPosition()const;
	float3 GetEulerRotation()const;


	static inline ComponentType GetType() { return ComponentType::TRANSFORM; };

	void SetPosition(float3 position);
	void SetScale(float3 scale);
	void SetTransform(float3 position,float3 scale ,Quat rotation); 
	void SetLocalTransform(float3 position, float3 scale, Quat rotation);
	void SetLocalTransform(float4x4 matrix);
	void SetRotation(Quat rotation);

	void UpdateLocalTransform();
	void SetEulerRotation(float3 eulerAngles);
	void UpdateTRS();

	void RecalculateEuler();

	//Update global transform
	void UpdatedTransform(float4x4 parentGlobalTransform);

private:

	float4x4 transform; //Local Transform
	float4x4 globalTransform;

	//Local
	float3 position;
	float3 scale;
	Quat rotation;

	float3 eulerRotation;

public:
	bool updateTransform = false; //should be private

};
#endif //__COMPONENTTRANSFORM__
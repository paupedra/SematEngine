#ifndef __COMPONENTTRANSFORM__
#define __COMPONENTTRANSFORM__

#include "Dependecies/MathGeoLib/src/MathGeoLib.h"
#include "glmath.h"

class Component;

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* owner);
	~ComponentTransform();

	void Update() override;
	void CleanUp() override;
	void DrawInspector() override;

	float4x4 GetTransform() const;
	float3 GetPosition()const;
	float3 GetScale()const;
	float4x4 GetGlobalTransform()const;

	static inline ComponentType GetType() { return ComponentType::TRANSFORM; };

	void SetPosition(float3 position);
	void SetScale(float3 scale);
	void SetTransform(float3 position,float3 scale ,Quat rotation); 

	void UpdateLocalTransform();
	void SetEulerRotation(float3 euler_angles);
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
	float3 eulerRotationUi;

public:
	bool updateTransform = false; //should be private

};
#endif //__COMPONENTTRANSFORM__
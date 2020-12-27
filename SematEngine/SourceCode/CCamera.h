
#include "Dependecies/MathGeoLib/include/Geometry/Plane.h"
#include "Dependecies/MathGeoLib/include/Geometry/Frustum.h"
#include "Dependecies/MathGeoLib/include/Math/float4x4.h"


class Component;

class CCamera : public Component
{
public:
	CCamera(GameObject* owner);

	void Update(float dt)override;
	void CleanUp() override;

	static inline ComponentType GetType() { return ComponentType::CAMERA; };

	//near/far planes

	void Setposition(float3 pos);

	void SetNearPlane(float distance);
	void SetFarPlane(float distance);

	void SetVerticalFov(float verticalFov);
	void SetHorizontalFov(float horizontalFov);
	void SetAspectRatio(float ratio);

	float ComputeAspectRatio(float verticalFov, float horizontalFov);

	void OnUpdateTransform(float4x4 globalTransform);

	//get view/projection
	float* GetViewMatrix();

	void UpdatePlanes();

	Frustum GetFrustum()const;
	float3 GetPos()const;
	float GetNearPlaneDistance()const;
	float GetFarPlaneDistance()const;
	float GetVerticalFov()const;
	float GetHorizontalFov()const;


public:
	
	Frustum frustum;
	bool isCurrentCamera = false;
	bool isCullingCamera = false;
	bool cull = true;
	Plane planes[6];
	vec* corners;
};
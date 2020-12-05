
//#include "Dependecies/MathGeoLib/include/Geometry/Plane.h"
//#include "Dependecies/MathGeoLib/include/Geometry/Frustum.h"

#include "Dependecies/MathGeoLib/include/MathGeoLib.h"

class Component;

class CCamera : public Component
{
public:
	CCamera(GameObject* owner);

	void Update()override;
	void CleanUp() override;

	//near/far planes

	void Setposition(float3 pos);

	void SetNearPlane(float distance);
	void SetFarPlane(float distance);

	void SetVerticalFov(float verticalFov);
	void SetHorizontalFov(float horizontalFov);

	float ComputeAspectRatio(float verticalFov, float horizontalFov);

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
	bool cull = true;
	Plane planes[6];
};
#include "Dependecies/MathGeoLib/include/Geometry/Frustum.h"

class Component;

class CCamera : public Component
{
public:
	CCamera(GameObject* owner);

	void Update()override;
	void CleanUp() override;

	//near/far planes

	void Setposition(float3 pos);

	Frustum GetFrustum()const;
	float3 GetPos()const;

	//get view/projection
	float* GetViewMatrix();

public:
	
	Frustum frustum;
	bool isCurrentCamera = false;
};

#include "Dependecies/MathGeoLib/src/MathGeoLib.h"
class Component;

class CCamera : public Component
{
public:
	CCamera(GameObject* owner);

	//near/far planes



	//get view/projection



	void Update()override;
	void CleanUp() override;

public:
	
	Frustum frustum;
	bool currentCamera = false;
};
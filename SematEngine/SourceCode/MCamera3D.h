#pragma once
#include "Module.h"
#include "Globals.h"
#include "Dependecies/MathGeoLib/include/Math/float4x4.h"
#include "Dependecies/MathGeoLib/include/Math/float3.h"
#include "Dependecies/MathGeoLib/include/Geometry/LineSegment.h"

class CCamera;
class GameObject;

class MCamera3D : public Module
{
public:
	MCamera3D(bool start_enabled = true);
	~MCamera3D();

	bool Start();
	updateStatus Update(float dt) override;
	bool CleanUp();

	void Look(const float3&Position, const float3&Reference, bool RotateAroundReference = false);
	void LookAt(const float3&Spot);
	void Move(const float3&Movement);
	void ZoomIn(float dt);
	void RotateCameraStatic();
	void FocusOnObject();
	void OrbitObject();

	void RaycastSelect();
	void CheckIntersetions(LineSegment* selectRay);

	float* GetRawViewMatrix();
	float4x4 GetViewMatrix();

	void SetCurrentCamera(CCamera* newCamera);
	void SetCullingCamera(CCamera* newCamera);

private:
	void CalculateViewMatrix();

public:
	GameObject* mainCameraObject = nullptr;		//Object holding mainCamera
	CCamera* cullingCamera = nullptr;				//Camera to be used outside Play Mode
	CCamera* currentCamera = nullptr;			//Camera currently being used, modify this camera to move

	float cameraMoveSpeed = 1;
	float cameraRotateSpeed = 1;

private:

	//float4x4 viewMatrix, viewMatrixInverse = float4x4::identity;
};
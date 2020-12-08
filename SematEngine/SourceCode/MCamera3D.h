#pragma once
#include "Module.h"
#include "Globals.h"
#include "Dependecies/MathGeoLib/include/Math/float4x4.h"
#include "Dependecies/MathGeoLib/include/Math/float3.h"

class CCamera;

class MCamera3D : public Module
{
public:
	MCamera3D(bool start_enabled = true);
	~MCamera3D();

	bool Start();
	update_status Update(float dt) override;
	bool CleanUp();

	void Look(const float3&Position, const float3&Reference, bool RotateAroundReference = false);
	void LookAt(const float3&Spot);
	void Move(const float3&Movement);
	void ZoomIn(float dt);
	void RotateCameraStatic();
	void FocusOnObject();
	void OrbitObject();

	float* GetRawViewMatrix();
	float4x4 GetViewMatrix();

private:
	void CalculateViewMatrix();

public:
	CCamera* mainCamera = nullptr;

	float cameraMoveSpeed = 1;
	float cameraRotateSpeed = 1;

private:

	//float4x4 viewMatrix, viewMatrixInverse = float4x4::identity;
};
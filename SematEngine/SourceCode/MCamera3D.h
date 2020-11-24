#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class MCamera3D : public Module
{
public:
	MCamera3D(bool start_enabled = true);
	~MCamera3D();

	bool Start();
	update_status Update(float dt) override;
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	void ZoomIn(float dt);
	void RotateCameraStatic();
	void FocusOnObject();
	void OrbitObject();

	float* GetRawViewMatrix();
	mat4x4 GetViewMatrix();

private:

	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, position, reference;

	float cameraMoveSpeed;
	float cameraRotateSpeed;

private:

	mat4x4 viewMatrix, viewMatrixInverse;
};
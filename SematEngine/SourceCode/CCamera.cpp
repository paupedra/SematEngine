#include "Component.h"
#include "Application.h"

#include "MRenderer3D.h"
#include "MCamera3D.h"

#include "CCamera.h"



CCamera::CCamera(GameObject* owner) : Component(ComponentType::CAMERA, owner)
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetPos(float3(0, 0, 0));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	frustum.SetViewPlaneDistances(0.1f, 10.0f);
	frustum.SetPerspective(1.0f, 1.0f);

	App->camera->currentCamera = this;
	isCurrentCamera = true;
}

void CCamera::Update()
{
	if(!isCurrentCamera)
		App->renderer3D->DrawFrustum(frustum);
}

void CCamera::CleanUp()
{

}

float* CCamera::GetViewMatrix()
{
	float4x4 mat = frustum.ViewMatrix();
	return (float*)mat.Transposed().v; //v is array pointer
}

void CCamera::Setposition(float3 pos)
{
	frustum.SetPos(pos);
}

Frustum CCamera::GetFrustum() const
{
	return frustum;
}

float3 CCamera::GetPos()const
{
	return frustum.Pos();
}
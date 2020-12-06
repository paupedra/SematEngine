#include "Component.h"
#include "Application.h"

#include "MRenderer3D.h"
#include "MCamera3D.h"

#include "CCamera.h"

#include "Dependecies/mmgr/mmgr.h"

CCamera::CCamera(GameObject* owner) : Component(ComponentType::CAMERA, owner)
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetPos(float3(0, 0, 0));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	frustum.SetViewPlaneDistances(0.1f, 10.0f);
	frustum.SetPerspective(1.0f, 1.0f);

	UpdatePlanes();

	//App->camera->mainCamera = this;
	//isCurrentCamera = true;

	App->renderer3D->currentCamera = this;

	corners = new vec[8];
}

void CCamera::Update()
{
	if (!isCurrentCamera)
	{
		
		frustum.GetCornerPoints(corners);
		App->renderer3D->DrawBox(corners);

	}

	
}

void CCamera::CleanUp()
{
	delete[] corners;
}

float* CCamera::GetViewMatrix()
{
	float4x4 mat = frustum.ViewMatrix();
	return (float*)mat.Transposed().v; //v is array pointer
}

void CCamera::UpdatePlanes()
{
	frustum.GetPlanes(planes);
}

void CCamera::Setposition(float3 pos)
{
	frustum.SetPos(pos);
	UpdatePlanes();
}

void CCamera::SetNearPlane(float distance)
{
	frustum.SetViewPlaneDistances(distance,GetFarPlaneDistance());
	UpdatePlanes();
}

void CCamera::SetFarPlane(float distance)
{
	frustum.SetViewPlaneDistances(GetNearPlaneDistance(), distance);
	UpdatePlanes();
}

void CCamera::SetVerticalFov(float verticalFov)
{
	frustum.SetVerticalFovAndAspectRatio(verticalFov, frustum.AspectRatio());
}

void CCamera::SetHorizontalFov(float horizontalFov)
{
	frustum.SetHorizontalFovAndAspectRatio(horizontalFov,frustum.AspectRatio());
}

float CCamera::ComputeAspectRatio(float verticalFov,float horizontalFov)
{
	return (Tan(verticalFov / 2) / Tan(horizontalFov / 2));
}

Frustum CCamera::GetFrustum() const
{
	return frustum;
}

float3 CCamera::GetPos()const
{
	return frustum.Pos();
}

float CCamera::GetNearPlaneDistance()const
{
	return frustum.NearPlaneDistance();
}

float CCamera::GetFarPlaneDistance()const
{
	return frustum.FarPlaneDistance();
}

float CCamera::GetVerticalFov()const
{
	return frustum.VerticalFov();
}

float CCamera::GetHorizontalFov()const
{
	return frustum.HorizontalFov();
}
#include "Component.h"
#include "Config.h"
#include "Application.h"
#include "GameObject.h"

#include "MRenderer3D.h"
#include "MCamera3D.h"
#include "MWindow.h"

#include "CCamera.h"
#include "CTransform.h"

//#include "Dependecies/mmgr/mmgr.h"

CCamera::CCamera(GameObject* owner) : Component(ComponentType::CAMERA, owner)
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetPos(float3(0, 0, 0));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	frustum.SetViewPlaneDistances(0.1f, 1000.0f);
	frustum.SetPerspective(1.0f, 1.0f);

	SetAspectRatio((float)App->window->GetWidth() / (float)App->window->GetHeight());
	SetVerticalFov(50.0f);

	UpdatePlanes();

	corners = new vec[8];
}

void CCamera::Update(float dt)
{
	if (!isCurrentCamera)
	{
		frustum.GetCornerPoints(corners);
		App->renderer3D->DrawBox(corners);
	}

	//frustum.ComputeViewMatrix();
}

void CCamera::CleanUp()
{
	delete[] corners;
}

void CCamera::Serialize(JsonNode* node)
{
	node->AddBool("Is Current Camera", isCurrentCamera);
	node->AddBool("Is Culling Camera", isCullingCamera);
}

void CCamera::Load(JsonNode* node)
{
	if (node->GetBool("Is Current Camera"))
		App->camera->SetCurrentCamera(this);
	if (node->GetBool("Is Culling Camera"))
		App->camera->SetCullingCamera(this);
}

float* CCamera::GetViewMatrix()
{
	static float4x4 m;

	m = frustum.ViewMatrix();

	m.Transpose();

	return (float*)m.v;
}

void CCamera::OnUpdateTransform(float4x4 globalTransform)
{
	/*frustum.SetFront(owner->transform->GetGlobalTransform().WorldZ());
	frustum.SetUp(owner->transform->GetGlobalTransform().WorldY());

	float3 position, scale = float3::zero;
	Quat rotation = Quat::identity;
	owner->transform->GetGlobalTransform().Decompose(position, rotation, scale);

	frustum.SetPos(position);
	UpdatePlanes();*/
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

void CCamera::SetVerticalFov(float verticalFov) //fov
{
	frustum.SetVerticalFovAndAspectRatio(verticalFov * DEGTORAD, ((float)App->window->GetWidth() / (float)App->window->GetHeight())); //win width / win height
}

void CCamera::SetHorizontalFov(float horizontalFov)
{
	frustum.SetHorizontalFovAndAspectRatio(horizontalFov, ((float)App->window->GetWidth() / (float)App->window->GetHeight()));
}

void CCamera::SetAspectRatio(float ratio)
{
	frustum.SetHorizontalFovAndAspectRatio(frustum.HorizontalFov(), ratio);
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
	return frustum.VerticalFov() * RADTODEG;
}

float CCamera::GetHorizontalFov()const
{
	return frustum.HorizontalFov();
}
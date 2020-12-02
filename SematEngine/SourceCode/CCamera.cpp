#include "Component.h"
#include "Application.h"

#include "MRenderer3D.h"
#include "CCamera.h"

CCamera::CCamera(GameObject* owner) : Component(ComponentType::CAMERA, owner)
{
	
}

void CCamera::Update()
{
	float3 corners[8];
	frustum.GetCornerPoints(corners);
	App->renderer3D->DrawBox(corners, float4x4::identity);
}

void CCamera::CleanUp()
{

}
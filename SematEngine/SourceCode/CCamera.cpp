#include "Component.h"

#include "CCamera.h"

CCamera::CCamera(GameObject* owner) : Component(ComponentType::CAMERA, owner)
{

}

void CCamera::Update()
{

}
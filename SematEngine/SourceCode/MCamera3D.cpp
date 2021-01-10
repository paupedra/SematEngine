#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include <map>

#include "MCamera3D.h"
#include "MInput.h"
#include "MScene.h"
#include "MRenderer3D.h"
#include "MWindow.h"
#include "MEditor.h"

#include "CTransform.h"
#include "CCamera.h"

#include "Dependecies/SDL/include/SDL.h"
//#include "Dependecies/mmgr/mmgr.h"

#define _USE_MATH_DEFINES

#include <math.h>

MCamera3D::MCamera3D(bool start_enabled) : Module(start_enabled)
{
	
}

MCamera3D::~MCamera3D()
{}

// -----------------------------------------------------------------
bool MCamera3D::Start()
{
	currentCamera = new CCamera(nullptr);

	cameraMoveSpeed = 7.5f;
	cameraRotateSpeed = 60.f;

	SetCurrentCamera(currentCamera);
	SetCullingCamera(currentCamera);

	currentCamera->Setposition(float3(0, 5,-5));

	LOG("Setting up the camera");
	bool ret = true;

	mainCameraObject = App->scene->CreateGameObject("MainCameraObject",App->scene->rootObject);
	mainCameraObject->AddComponent(currentCamera);

	return ret;
}

// -----------------------------------------------------------------
bool MCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
updateStatus MCamera3D::Update(float dt)
{
	float3 newPos(0,0,0);
	float speed = cameraMoveSpeed * dt;

	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = (cameraMoveSpeed * 2.f) * dt;

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += currentCamera->frustum.Front() * speed;
	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= currentCamera->frustum.Front() * speed;

	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= currentCamera->frustum.WorldRight() * speed;
	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += currentCamera->frustum.WorldRight() * speed;

	FocusOnObject();

	ZoomIn(dt);

	if (!App->editor->IsMouseHovering())
	{
		/*if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			RaycastSelect();*/
	}

	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_IDLE)
		RotateCameraStatic();

	/*if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		OrbitObject();*/

	
	currentCamera->Setposition(currentCamera->frustum.Pos() + newPos);

	return UPDATE_CONTINUE;
}

void MCamera3D::SetPosition(float3 newPosition)
{
	if (currentCamera != nullptr)
	{
		currentCamera->Setposition(newPosition);
	}
}

void MCamera3D::RaycastSelect()
{
	float2 mousePos = float2(App->input->GetMouseX(), App->window->GetHeight() - App->input->GetMouseY());

	float mouseNormalX = mousePos.x / App->window->GetWidth();
	float mouseNormalY = mousePos.y / App->window->GetHeight();

	mouseNormalX = (mouseNormalX - 0.5) / 0.5;
	mouseNormalY = (mouseNormalY - 0.5) / 0.5;

	LineSegment selectRay = currentCamera->frustum.UnProjectLineSegment(mouseNormalX, mouseNormalY);

	App->renderer3D->DrawLine(selectRay.a, selectRay.b);

	//LOG("Ray goes from: %d to %d", selectRay.a, selectRay.b);

	CheckIntersetions(&selectRay);
}

void MCamera3D::CheckIntersetions(LineSegment* selectRay)
{
	bool objectFound = false;
	std::map<float, GameObject*> hits;

	for (std::vector<GameObject*>::iterator object = App->scene->gameObjects.begin(); object != App->scene->gameObjects.end(); object++)
	{
		if (selectRay->Intersects( (*object)->AABB ) ) 
		{
			//LOG("Got a hit with: %s", (*object)->GetName());

			float nearInter, farInter;
			if (selectRay->Intersects((*object)->OBB, nearInter, farInter)) //It should intersect 2 times?
			{
				hits.emplace(nearInter, (*object));
				objectFound = true;
			}
		}
	}

	if (objectFound)
		App->scene->SetSelectedObject((*hits.begin()).second);
	//else
		//App->scene->SetSelectedObject(nullptr);

}

void MCamera3D::RotateCameraStatic()
{
	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	float sensitivity = 0.003f;
	//looking = false;

	// x motion make the camera rotate in Y absolute axis (0,1,0) (not local)
	if (dx != 0.f)
	{
		Quat q = Quat::RotateY(dx * sensitivity);
		currentCamera->frustum.SetFront(q.Mul(currentCamera->frustum.Front()).Normalized());
		currentCamera->frustum.SetUp(q.Mul(currentCamera->frustum.Up()).Normalized());
	}

	// y motion makes the camera rotate in X local axis, with tops
	if (dy != 0.f)
	{
		Quat q = Quat::RotateAxisAngle(currentCamera->frustum.WorldRight(), dy * sensitivity);

		vec newUp = q.Mul(currentCamera->frustum.Up()).Normalized();

		if (newUp.y > 0.0f)
		{
			currentCamera->frustum.SetUp(newUp);
			currentCamera->frustum.SetFront(q.Mul(currentCamera->frustum.Front()).Normalized());
		}
	}
}

void MCamera3D::FocusOnObject()
{
	/*if (App->scene->selectedObject != nullptr)
	{
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{

			reference.x = App->scene->selectedObject->transform->GetPosition().x;
			reference.y = App->scene->selectedObject->transform->GetPosition().y;
			reference.z = App->scene->selectedObject->transform->GetPosition().z;
			position = reference + Z * 10;
		}
	}*/
}



void MCamera3D::OrbitObject()
{
	//int dx = -App->input->GetMouseXMotion();
	//int dy = -App->input->GetMouseYMotion();

	//float Sensitivity = 0.15f;

	//if (App->scene->selectedObject != nullptr)
	//{
	//	reference.x = App->scene->selectedObject->transform->GetPosition().x;
	//	reference.y = App->scene->selectedObject->transform->GetPosition().y;
	//	reference.z = App->scene->selectedObject->transform->GetPosition().z;
	//}

	//position -= reference;

	//if (dx != 0)
	//{
	//	float DeltaX = (float)dx * Sensitivity;

	//	float4x4 mat;


	//	X = rotate(X, DeltaX, float3(0.0f, 1.0f, 0.0f));
	//	Y = rotate(Y, DeltaX, float3(0.0f, 1.0f, 0.0f));
	//	Z = rotate(Z, DeltaX, float3(0.0f, 1.0f, 0.0f));
	//}

	//if (dy != 0)
	//{
	//	float DeltaY = (float)dy * Sensitivity;

	//	Y = rotate(Y, DeltaY, X);
	//	Z = rotate(Z, DeltaY, X);

	//	if (Y.y < 0.0f)
	//	{
	//		Z = float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
	//		Y = Z.Cross(X);
	//		
	//	}
	//}

	//position = reference + Z * position.Length();
}

void MCamera3D::Look(const float3& Position, const float3& Reference, bool RotateAroundReference)
{
	/*this->position = Position;
	this->reference = Reference;

	Z = (Position - Reference).Normalized();
	X = (float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
	Y = Z.Cross(X);

	if(!RotateAroundReference)
	{
		this->reference = this->position;
		this->position += Z * 0.05f;
	}*/
}

// -----------------------------------------------------------------
void MCamera3D::LookAt(const float3& Spot)
{
	/*reference = Spot;


	Z = (position - reference).Normalized();
	X = (float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
	Y = Z.Cross(X);*/
}

// -----------------------------------------------------------------
void MCamera3D::Move(const float3& Movement)
{
	//position += Movement;
	//reference += Movement;
}

void MCamera3D::ZoomIn(float dt)
{
	//int scrollWheel = App->input->GetMouseZ(); // -1 if moving towards me 1 if moving backwards
	//float speed = cameraRotateSpeed * dt;

	//if (scrollWheel > 0)
	//{
	//	mainCamera->Setposition(mainCamera->GetPos() - (Z * speed));
	//}
	//if (scrollWheel < 0)
	//{
	//	mainCamera->Setposition(mainCamera->GetPos() + (Z * speed));
	//}
}

// -----------------------------------------------------------------
float* MCamera3D::GetRawViewMatrix()
{
	float4x4 viewMatrix = currentCamera->frustum.ViewMatrix();
	return (float*)&viewMatrix;
}

float4x4 MCamera3D::GetViewMatrix()
{
	return float4x4::identity;
}

// -----------------------------------------------------------------
void MCamera3D::CalculateViewMatrix()
{
	//viewMatrix = float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -X.Dot(position), -Y.Dot(position), -Z.Dot(position), 1.0f);
	//viewMatrixInverse = viewMatrix.Inverted();


}

void MCamera3D::SetCurrentCamera(CCamera* newCamera)
{
	currentCamera->isCurrentCamera = false;
	currentCamera = newCamera;
	currentCamera->isCurrentCamera = true;
}

void MCamera3D::SetCullingCamera(CCamera* newCamera)
{
	if(cullingCamera)
		cullingCamera->isCullingCamera = false;

	cullingCamera = newCamera;
	cullingCamera->isCullingCamera = true;
}
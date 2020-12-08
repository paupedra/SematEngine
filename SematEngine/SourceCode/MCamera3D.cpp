
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"

#include "MCamera3D.h"
#include "MInput.h"
#include "MScene.h"

#include "CTransform.h"
#include "CCamera.h"

#include "Dependecies/SDL/include/SDL.h"

#include "Dependecies/mmgr/mmgr.h"

#define _USE_MATH_DEFINES

#include <math.h>

MCamera3D::MCamera3D(bool start_enabled) : Module(start_enabled)
{
	mainCamera = new CCamera(nullptr);

	mainCamera->frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	mainCamera->frustum.SetPos(float3(0, 0, 0));
	mainCamera->frustum.SetFront(float3::unitZ);
	mainCamera->frustum.SetUp(float3::unitY);

	mainCamera->frustum.SetViewPlaneDistances(0.1f, 10.0f);
	mainCamera->frustum.SetPerspective(1.0f, 1.0f);

	cameraMoveSpeed = 7.5f;
	cameraRotateSpeed = 60.f;
}

MCamera3D::~MCamera3D()
{}

// -----------------------------------------------------------------
bool MCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool MCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status MCamera3D::Update(float dt)
{
	float3 newPos(0,0,0);
	float speed = cameraMoveSpeed * dt;

	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = (cameraMoveSpeed * 2.f) * dt;

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += mainCamera->frustum.Front() * speed;
	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= mainCamera->frustum.Front() * speed;


	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= mainCamera->frustum.WorldRight() * speed;
	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += mainCamera->frustum.WorldRight() * speed;

	FocusOnObject();

	ZoomIn(dt);

	//if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_IDLE)
		//RotateCameraStatic();

	/*if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		OrbitObject();*/

	mainCamera->Setposition(mainCamera->frustum.Pos() + newPos);

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void MCamera3D::Look(const float3&Position, const float3&Reference, bool RotateAroundReference)
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
void MCamera3D::LookAt( const float3&Spot)
{
	/*reference = Spot;
	

	Z = (position - reference).Normalized();
	X = (float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
	Y = Z.Cross(X);*/
}

// -----------------------------------------------------------------
void MCamera3D::Move(const float3&Movement)
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

float4x4 rotate(float angle, const float3& u)
{
	float4x4 Rotate;

	angle = angle / 180.0f * (float)M_PI;

	float3 v = u.Normalized();

	float c = 1.0f - cos(angle), s = sin(angle);

	Rotate.At(0, 0) = 1.0f + c * (v.x * v.x - 1.0f);
	Rotate.At(0, 1) = c * v.x * v.y + v.z * s;
	Rotate.At(0, 2) = c * v.x * v.z - v.y * s;
	Rotate.At(1, 0) = c * v.x * v.y - v.z * s;
	Rotate.At(1, 1) = 1.0f + c * (v.y * v.y - 1.0f);
	Rotate.At(1, 2) = c * v.y * v.z + v.x * s;
	Rotate.At(2, 0) = c * v.x * v.z + v.y * s;
	Rotate.At(2, 1) = c * v.y * v.z - v.x * s;
	Rotate.At(2, 2) = 1.0f + c * (v.z * v.z - 1.0f);

	return Rotate;
}

float3 rotate(const float3& u, float angle, const float3& v)
{
	return *(float3*)&(rotate(angle, v) * float4(u, 1.0f));
}

void MCamera3D::RotateCameraStatic()
{
	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	float Sensitivity = 0.15f;

	if (dx != 0)
	{
		/*float distance = reference.Distance(mainCamera->frustum.Pos());
		float3 Y_add = mainCamera->frustum.Up() * dy * (distance / 1800);
		float3 X_add = mainCamera->frustum.WorldRight() * -dx * (distance / 1800);

		reference += X_add;
		reference += Y_add;

		mainCamera->frustum.look*/

		/*X = rotate(X, DeltaX, float3(0.0f, 1.0f, 0.0f));
		Y = rotate(Y, DeltaX, float3(0.0f, 1.0f, 0.0f));
		Z = rotate(Z, DeltaX, float3(0.0f, 1.0f, 0.0f));*/
	}

	/*if (dy != 0)
	{
		float DeltaY = (float)dy * Sensitivity;

		Y = rotate(Y, DeltaY, X);
		Z = rotate(Z, DeltaY, X);

		if (Y.y < 0.0f)
		{
			Z = float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			Y = Z.Cross(X);
		}
	}*/

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

// -----------------------------------------------------------------
float* MCamera3D::GetRawViewMatrix()
{
	CalculateViewMatrix();
	float4x4 viewMatrix = mainCamera->frustum.ComputeViewMatrix();
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

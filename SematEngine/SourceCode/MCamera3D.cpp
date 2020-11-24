#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"

#include "MCamera3D.h"
#include "MInput.h"
#include "MScene.h"

#include "CTransform.h"

#include "Dependecies/mmgr/mmgr.h"

MCamera3D::MCamera3D(bool start_enabled) : Module(start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	position = vec3(0.0f, 0.0f, 5.0f);
	reference = vec3(0.0f, 0.0f, 0.0f);

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
	vec3 newPos(0,0,0);
	float speed = cameraMoveSpeed * dt;

	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = (cameraMoveSpeed * 2.f) * dt;

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	FocusOnObject();

	ZoomIn(dt);

	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_IDLE)
		RotateCameraStatic();

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		OrbitObject();

	position += newPos;
	reference += newPos;

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void MCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->position = Position;
	this->reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->reference = this->position;
		this->position += Z * 0.05f;
	}
}

// -----------------------------------------------------------------
void MCamera3D::LookAt( const vec3 &Spot)
{
	reference = Spot;

	Z = normalize(position - reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);
}

// -----------------------------------------------------------------
void MCamera3D::Move(const vec3 &Movement)
{
	position += Movement;
	reference += Movement;
}

void MCamera3D::ZoomIn(float dt)
{
	int scrollWheel = App->input->GetMouseZ(); // -1 if moving towards me 1 if moving backwards
	float speed = cameraRotateSpeed * dt;

	if (scrollWheel > 0)
	{
		position -= Z * speed;
	}
	if (scrollWheel < 0)
	{
		position += Z * speed;
	}
}

void MCamera3D::RotateCameraStatic()
{
	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	float Sensitivity = 0.15f;

	if (dx != 0)
	{
		float DeltaX = (float)dx * Sensitivity;

		X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	}

	if (dy != 0)
	{
		float DeltaY = (float)dy * Sensitivity;

		Y = rotate(Y, DeltaY, X);
		Z = rotate(Z, DeltaY, X);

		if (Y.y < 0.0f)
		{
			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			Y = cross(Z, X);
		}
	}

}

void MCamera3D::FocusOnObject()
{
	if (App->scene->selectedObject != nullptr)
	{
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{

			reference.x = App->scene->selectedObject->transform->GetPosition().x;
			reference.y = App->scene->selectedObject->transform->GetPosition().y;
			reference.z = App->scene->selectedObject->transform->GetPosition().z;
			position = reference + Z * 10;
		}
	}
}

void MCamera3D::OrbitObject()
{
	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	float Sensitivity = 0.15f;

	if (App->scene->selectedObject != nullptr)
	{
		reference.x = App->scene->selectedObject->transform->GetPosition().x;
		reference.y = App->scene->selectedObject->transform->GetPosition().y;
		reference.z = App->scene->selectedObject->transform->GetPosition().z;
	}

	position -= reference;

	if (dx != 0)
	{
		float DeltaX = (float)dx * Sensitivity;

		X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	}

	if (dy != 0)
	{
		float DeltaY = (float)dy * Sensitivity;

		Y = rotate(Y, DeltaY, X);
		Z = rotate(Z, DeltaY, X);

		if (Y.y < 0.0f)
		{
			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			Y = cross(Z, X);
		}
	}

	position = reference + Z * length(position);
		
}

// -----------------------------------------------------------------
float* MCamera3D::GetRawViewMatrix()
{
	CalculateViewMatrix();
	return &viewMatrix;
}

mat4x4 MCamera3D::GetViewMatrix()
{
	CalculateViewMatrix();
	return viewMatrix;
}

// -----------------------------------------------------------------
void MCamera3D::CalculateViewMatrix()
{
	viewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, position), -dot(Y, position), -dot(Z, position), 1.0f);
	viewMatrixInverse = inverse(viewMatrix);
}

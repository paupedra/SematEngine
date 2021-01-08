#include "Globals.h"
#include "GameObject.h"
#include "Component.h"
#include "Config.h"

#include "CTransform.h"

#include "Dependecies/imgui/imgui.h"

//#include "Dependecies/mmgr/mmgr.h"

CTransform::CTransform(GameObject* owner) : Component(ComponentType::TRANSFORM,owner)
{
	position = float3(0.f, 0.f, 0.f);
	scale = float3(1.f, 1.f, 1.f);

	eulerRotation = float3(0.f, 0.f, 0.f);
	rotation = Quat::identity;

	transform = float4x4::FromTRS(position, rotation, scale);
	globalTransform = Quat::identity;
}

CTransform::~CTransform()
{

}

void CTransform::Update(float dt)
{
	if (updateTransform)
	{
		owner->UpdatedTransform();
	}

	if (updateTransform)
	{
		LOG("UpdateTransform still true");
	}
}

void CTransform::CleanUp()
{

}

void CTransform::Serialize(JsonNode* node)
{
	JsonArray _position = node->InitArray("Position");
	_position.AddNumber(position.x);
	_position.AddNumber(position.y);
	_position.AddNumber(position.z);

	JsonArray _scale = node->InitArray("Scale");
	_scale.AddNumber(scale.x);
	_scale.AddNumber(scale.y);
	_scale.AddNumber(scale.z);

	JsonArray _rotation = node->InitArray("Rotation");
	_rotation.AddNumber(rotation.x);
	_rotation.AddNumber(rotation.y);
	_rotation.AddNumber(rotation.z);
	_rotation.AddNumber(rotation.w);
}

void CTransform::Load(JsonNode* node)
{
	JsonArray positionJson = node->GetArray("Position");
	float3 _position = {(float)positionJson.GetNumber(0),(float)positionJson.GetNumber(1),(float)positionJson.GetNumber(2) };

	JsonArray scaleJson = node->GetArray("Scale");
	float3 _scale = { (float)scaleJson.GetNumber(0),(float)scaleJson.GetNumber(1),(float)scaleJson.GetNumber(2) };

	JsonArray rotationJson = node->GetArray("Rotation");
	Quat _rotation = { (float)rotationJson.GetNumber(0),(float)rotationJson.GetNumber(1), (float)rotationJson.GetNumber(2), (float)rotationJson.GetNumber(3) };

	SetLocalTransform(_position, _scale, _rotation);
}

void CTransform::UpdateTRS()
{
	transform.Decompose(position, rotation, scale);
	RecalculateEuler();
}

void CTransform::SetEulerRotation(float3 eulerAngles)
{
	float3 delta = (eulerAngles - eulerRotation) * 0.0174532925199432957f;
	Quat quaternion_rotation = Quat::FromEulerXYZ(delta.x, delta.y, delta.z);
	rotation = rotation * quaternion_rotation;
	eulerRotation = eulerAngles;
	UpdateLocalTransform();
}

float4x4 CTransform::GetTransform() const
{
	return transform;
}

float3 CTransform::GetPosition()const
{
	return position;
}

float3 CTransform::GetScale()const
{
	return scale;
}

float4x4 CTransform::GetGlobalTransform()const
{
	return globalTransform;
}

float3 CTransform::GetGlobalPosition()const
{
	float3 position = float3::zero;
	Quat rotation = Quat::identity;
	float3 scale = float3::zero;
	globalTransform.Decompose(position,rotation,scale);

	return position;
}

float3 CTransform::GetEulerRotation()const
{
	return eulerRotation;
}

void CTransform::SetPosition(float3 position)
{
	this->position = position;
	UpdateLocalTransform();
}

void CTransform::SetScale(float3 scale)
{
	this->scale = scale;
	UpdateLocalTransform();
}

void CTransform::SetRotation(Quat rotation)
{
	this->rotation = rotation;
	UpdateLocalTransform();
}

void CTransform::SetLocalTransform(float3 position, float3 scale, Quat rotation)
{
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
	UpdateLocalTransform();
}

void CTransform::SetLocalTransform(float4x4 matrix)
{
	matrix.Decompose(position,rotation,scale);

	UpdateLocalTransform();
}

void CTransform::SetTransform(float3 position, float3 scale, Quat rotation)
{

}

void CTransform::UpdateLocalTransform()
{
	transform = float4x4::FromTRS(position, rotation, scale);
	RecalculateEuler();
	if(owner->parent != nullptr)
		updateTransform = true;
}

void CTransform::RecalculateEuler()
{
	eulerRotation = rotation.ToEulerXYZ();
	eulerRotation *= 57.295779513082320876f;
}

void CTransform::UpdatedTransform(float4x4 parentGlobalTransform)
{
	globalTransform = parentGlobalTransform * transform;
	UpdateTRS();

	updateTransform = false;
}

void CTransform::OnSave(JsonNode* node)
{
	
}
#include "Globals.h"
#include "GameObject.h"
#include "Component.h"
#include "Config.h"

#include "CTransform.h"

#include "Dependecies/imgui/imgui.h"

#include "Dependecies/mmgr/mmgr.h"

CTransform::CTransform(GameObject* owner) : Component(ComponentType::TRANSFORM,owner)
{
	position = float3(0.f, 0.f, 0.f);
	scale = float3(1.f, 1.f, 1.f);

	eulerRotationUi = float3(0.f, 0.f, 0.f);
	eulerRotation = float3(0.f, 0.f, 0.f);
	rotation = Quat::identity;

	transform = float4x4::FromTRS(position, rotation, scale);
	globalTransform = Quat::identity;
}

CTransform::~CTransform()
{

}

void CTransform::Update()
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

void CTransform::UpdateTRS()
{
	transform.Decompose(position, rotation, scale);
	RecalculateEuler();
}

void  CTransform::DrawInspector()
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;

		if (ImGui::InputFloat3("Transform", (float*)&position, "%.2f", flags)) { UpdateLocalTransform(); };
		if (ImGui::InputFloat3("Scale", (float*)&scale, "%.2f", flags)) { UpdateLocalTransform(); };
		if (ImGui::InputFloat3("Rotation", (float*)&eulerRotationUi, "%.2f",flags)) { SetEulerRotation(eulerRotationUi); }
	}
}

void CTransform::OnSave(ConfigNode* node)
{
	ConfigArray _position = node->InitArray("Position");
	_position.AddNumber(position.x);
	_position.AddNumber(position.y);
	_position.AddNumber(position.z);

	ConfigArray _scale = node->InitArray("Scale");
	_scale.AddNumber(scale.x);
	_scale.AddNumber(scale.y);
	_scale.AddNumber(scale.z);

	ConfigArray _rotation = node->InitArray("Rotation");
	_rotation.AddNumber(rotation.x);
	_rotation.AddNumber(rotation.y);
	_rotation.AddNumber(rotation.z);
	_rotation.AddNumber(rotation.w);
}

void CTransform::SetEulerRotation(float3 euler_angles)
{
	float3 delta = (euler_angles - eulerRotation) * 0.0174532925199432957f;
	Quat quaternion_rotation = Quat::FromEulerXYZ(delta.x, delta.y, delta.z);
	rotation = rotation * quaternion_rotation;
	eulerRotation = euler_angles;
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

void CTransform::SetLocalTransform(float3 position, float3 scale, Quat rotation)
{
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
	UpdateLocalTransform();
}

void CTransform::SetTransform(float3 position, float3 scale, Quat rotation)
{

}

void CTransform::UpdateLocalTransform()
{
	transform = float4x4::FromTRS(position, rotation, scale);
	//RecalculateEuler();
	updateTransform = true;
}

void CTransform::RecalculateEuler()
{
	eulerRotation = rotation.ToEulerXYZ();
	eulerRotation *= 57.295779513082320876f;
	eulerRotationUi = eulerRotation;
}

void CTransform::UpdatedTransform(float4x4 parentGlobalTransform)
{
	globalTransform = parentGlobalTransform * transform;
	UpdateTRS();

	//LOG("Updated Transform of: %s", owner->GetName());

	updateTransform = false;
	//LOG("set update transform to false -------------");
}
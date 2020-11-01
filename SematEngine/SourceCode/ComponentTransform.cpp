
#include "Component.h"
#include "ComponentTransform.h"
#include "Dependecies/imgui/imgui.h"
#include "GameObject.h"

ComponentTransform::ComponentTransform(GameObject* owner) : Component(ComponentType::TRANSFORM,owner)
{
	position = float3(0.f, 0.f, 0.f);
	positionUI = float3(0.f, 0.f, 0.f);
	scale = float3(1.f, 1.f, 1.f);
	eulerRotationUi = float3(0.f, 0.f, 0.f);
	eulerRotation = float3(0.f, 0.f, 0.f);
	rotation = Quat::identity;
	transform = float4x4::FromTRS(position, rotation, scale);
}

ComponentTransform::~ComponentTransform()
{

}

void ComponentTransform::Update()
{
	
	//RecalculateMatrix();
	//transform.Decompose(position, rotation, scale);
	//RecalculateEuler();
}

void ComponentTransform::UpdateTRS()
{
	transform.Decompose(position, rotation, scale);
	RecalculateEuler();
}

void  ComponentTransform::DrawInspector()
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;

		ImGui::InputFloat3("Transform", (float*)&positionUI, "%.2f", flags);
		ImGui::InputFloat3("Scale", (float*)&scale, "%.2f", flags);
		if (ImGui::InputFloat3("Rotation", (float*)&eulerRotationUi, "%.2f", flags)) { SetEulerRotation(eulerRotationUi); }
	}
}

void ComponentTransform::SetEulerRotation(float3 euler_angles)
{
	float3 delta = (euler_angles - eulerRotation) * 0.0174532925199432957f;
	Quat quaternion_rotation = Quat::FromEulerXYZ(delta.x, delta.y, delta.z);
	rotation = rotation * quaternion_rotation;
	eulerRotation = euler_angles;
	RecalculateMatrix();
}

float4x4 ComponentTransform::GetTransform() const
{
	return transform;
}

float3 ComponentTransform::GetPosition()const
{
	return position;
}

float3 ComponentTransform::GetScale()const
{
	return scale;
}

void ComponentTransform::SetPosition(float3 position)
{
	this->position = position;
	RecalculateMatrix();
}

void ComponentTransform::SetScale(float3 scale)
{
	this->scale = scale;
	RecalculateMatrix();
}

void ComponentTransform::RecalculateMatrix()
{
	transform = float4x4::FromTRS(position, rotation, scale);
}

void ComponentTransform::RecalculateEuler()
{
	eulerRotation = rotation.ToEulerXYZ();
	eulerRotation *= 57.295779513082320876f;
}
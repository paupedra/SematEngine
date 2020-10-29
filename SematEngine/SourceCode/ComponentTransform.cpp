
#include "Component.h"
#include "ComponentTransform.h"
#include "Dependecies/imgui/imgui.h"

ComponentTransform::ComponentTransform(GameObject* owner) : Component(ComponentType::TRANSFORM,owner)
{
	position = float3(0.f, 0.f, 0.f);
	scale = float3(1.f, 1.f, 1.f);
	rotation = Quat::identity;
	transform = float4x4::FromTRS(position, rotation, scale);
}

ComponentTransform::~ComponentTransform()
{

}

void ComponentTransform::Update()
{

}

void  ComponentTransform::DrawInspector()
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::InputFloat3("Transform", (float*)&position, 1);
		ImGui::InputFloat3("Scale", (float*)&scale, 1);
	}
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
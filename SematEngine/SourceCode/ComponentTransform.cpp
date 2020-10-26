
#include "Component.h"
#include "ComponentTransform.h"

ComponentTransform::ComponentTransform()
{

}

ComponentTransform::~ComponentTransform()
{

}

void ComponentTransform::Update()
{

}

mat4x4 ComponentTransform::GetTransform() const
{
	return transform;
}
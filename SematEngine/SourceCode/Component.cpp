#include "Component.h"
#include "GameObject.h"

Component::Component(GameObject* owner) : owner(owner)
{

}

Component::~Component()
{

}

void Component::Update()
{}

void Component::Enable()
{

}

void Component::Disable()
{

}

bool Component::IsActive()
{
	return active;
}

type Component::GetType()
{
	return type;
}

GameObject* Component::GetOwner()
{
	return owner;
}
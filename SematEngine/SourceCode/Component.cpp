#include "Component.h"
#include "GameObject.h"

Component::Component()
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
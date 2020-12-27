#include "Component.h"
#include "GameObject.h"

//#include "Dependecies/mmgr/mmgr.h"

Component::Component(ComponentType type) : type(type)
{

}

Component::Component(ComponentType type,GameObject* owner) : owner(owner) , type(type)
{

}

Component::~Component()
{

}

void Component::Update(float dt)
{

}

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

//Component::ComponentType Component::GetType()
//{
//	return type;
//}

GameObject* Component::GetOwner()
{
	return owner;
}
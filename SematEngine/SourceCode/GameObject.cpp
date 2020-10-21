#include "GameObject.h"

GameObject::GameObject()
{

}
GameObject::~GameObject()
{

}

void GameObject::Update()
{

}

void GameObject::Enable()
{
	active = true;
}

void GameObject::Disable()
{
	active = false;
}

bool GameObject::IsActive()
{
	return active;
}

const char* GameObject::GetName()
{
	return name.c_str();
}

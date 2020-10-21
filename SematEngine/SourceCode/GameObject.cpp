#include "GameObject.h"
#include "Component.h"

GameObject::GameObject()
{

}
GameObject::~GameObject()
{

}

void GameObject::Update()
{
	std::vector<Component*>::iterator item = components.begin();
	for (; item != components.end(); ++item)
	{
		(*item)->Update();
	}

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

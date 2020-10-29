#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"

GameObject::GameObject(char* name) : name(name)
{
	AddComponent(new ComponentTransform(this));
}

GameObject::GameObject(GameObject* parent = nullptr, char* name = "Object") : parent(parent), name(name)
{
	AddComponent(new ComponentTransform(this));
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

	std::vector<GameObject*>::iterator child = children.begin();
	for (; child != children.end(); ++child)
	{
		(*child)->Update();
	}
}

Component* GameObject::AddComponent(Component* component)
{
	ComponentType type = component->GetType();
	
	switch (type)
	{
		case TRANSFORM:
			components.push_back(component);
			transform = (ComponentTransform*)component;
			break;

		case MESH:
			
			components.push_back(component);
			
			break;
	}

	return component;
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
std::vector<Component*> GameObject::GetComponents()const
{
	return components;
}
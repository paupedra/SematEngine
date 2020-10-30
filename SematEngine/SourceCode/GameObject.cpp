#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "Globals.h"

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
			
			if (!HasComponentType(MESH))
			{
				components.push_back(component);
			}
			else
				LOG("(ERROR) Error adding Mesh: Object already has Mesh");
			
			break;

		case TEXTURE:

			if (!HasComponentType(TEXTURE))
			{
				components.push_back(component);
				texture = (ComponentTexture*)component;
			}
			else
			{
				DeleteComponentType(TEXTURE);
				components.push_back(component);
				texture = (ComponentTexture*)component;
			}

			break;
	}

	return component;
}

void GameObject::DeleteComponentType(ComponentType type)
{
	std::vector<Component*>::iterator item = components.begin();
	for (; item != components.end(); ++item)
	{
		if ((*item)->GetType() == type)
		{
			components.erase(item);
			return;

		}
	}
}

bool GameObject::HasComponentType(ComponentType type)
{
	bool ret = false;
	std::vector<Component*>::iterator item = components.begin();
	for (; item != components.end(); ++item)
	{
		if ((*item)->GetType() == type)
		{
			ret = true;
			return ret;
		}
	}

	return ret;
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
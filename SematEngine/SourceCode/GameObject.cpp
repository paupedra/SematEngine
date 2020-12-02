#include "Globals.h"
#include "GameObject.h"
#include "Component.h"

#include "CTransform.h"
#include "CMesh.h"

#include "Dependecies/mmgr/mmgr.h"

GameObject::GameObject(const char* name) : name(name)
{
	AddComponent(new CTransform(this));
}

GameObject::GameObject(GameObject* parent = nullptr, const char* name = "Object") : parent(parent), name(name)
{
	AddComponent(new CTransform(this));
}

GameObject::~GameObject()
{

}

void GameObject::Update()
{
	if (!components.empty())
	{
		std::vector<Component*>::iterator item = components.begin();
		for (; item != components.end(); ++item)
		{
			(*item)->Update();
		}
	}
}

void GameObject::CleanUp()
{

	std::vector<Component*>::iterator item = components.begin();
	for (; item != components.end(); ++item)
	{
		(*item)->CleanUp();
		delete (*item);
	}

	components.clear();

	children.clear();
}

Component* GameObject::AddComponent(Component* component)
{
	ComponentType type = component->GetType();
	
	switch (type)
	{
	case ComponentType::TRANSFORM:

			components.push_back(component);
			transform = (CTransform*)component;
			break;

		case ComponentType::MESH:
			
			if (!HasComponentType(ComponentType::MESH))
			{
				components.push_back(component);
			}
			else
				LOG("(ERROR) Error adding Mesh: Object already has Mesh");
			
			break;

		case ComponentType::MATERIAL:

			if (!HasComponentType(ComponentType::MATERIAL))
			{
				components.push_back(component);
			}
			else
			{
				DeleteComponentType(ComponentType::MATERIAL);
				components.push_back(component);
			}

			break;

		case ComponentType::CAMERA:

			if (!HasComponentType(ComponentType::CAMERA))
			{
				components.push_back(component);
			}
			else
				LOG("(ERROR) Error adding Camera: Object already has camera");

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

void GameObject::SetName(const char* name)
{
	this->name = name;
}

std::vector<Component*> GameObject::GetComponents()const
{
	return components;
}

void GameObject::UpdatedTransform()
{
	transform->UpdatedTransform(parent->transform->GetGlobalTransform());

	//call children's on updateTransforms
	std::vector<GameObject*>::iterator child = children.begin();
	for (; child != children.end(); ++child)
	{
		(*child)->UpdatedTransform();
	}
}
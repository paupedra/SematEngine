#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "Globals.h"
#include "Dependecies/mmgr/mmgr.h"

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
	if (!components.empty())
	{
		std::vector<Component*>::iterator item = components.begin();
		for (; item != components.end(); ++item)
		{
			(*item)->Update();
		}
	}

	std::vector<GameObject*>::iterator child = children.begin();
	for (; child != children.end(); ++child)
	{
		(*child)->Update();
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

	//std::vector<GameObject*>::iterator child = children.begin();
	//for (; child != children.end(); ++child)
	//{
	//	(*child)->CleanUp();
	//	delete (*child);
	//}

	children.clear();
}

Component* GameObject::AddComponent(Component* component)
{
	Component::ComponentType type = component->GetType();
	
	switch (type)
	{
	case Component::ComponentType::TRANSFORM:

			components.push_back(component);
			transform = (ComponentTransform*)component;
			break;

		case Component::ComponentType::MESH:
			
			if (!HasComponentType(Component::ComponentType::MESH))
			{
				components.push_back(component);
			}
			else
				LOG("(ERROR) Error adding Mesh: Object already has Mesh");
			
			break;

		case Component::ComponentType::TEXTURE:

			if (!HasComponentType(Component::ComponentType::TEXTURE))
			{
				components.push_back(component);
			}
			else
			{
				DeleteComponentType(Component::ComponentType::TEXTURE);
				components.push_back(component);
			}

			break;
	}

	return component;
}

void GameObject::DeleteComponentType(Component::ComponentType type)
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

bool GameObject::HasComponentType(Component::ComponentType type)
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

void GameObject::UpdatedTransform()
{
	
	//If parent update 
	transform->UpdatedTransform(parent->transform->GetGlobalTransform());


	//call children's on updateTransforms
	
	std::vector<GameObject*>::iterator child = children.begin();
	for (; child != children.end(); ++child)
	{
		(*child)->UpdatedTransform();
	}
}
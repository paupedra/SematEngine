#include "Globals.h"
#include "GameObject.h"
#include "Component.h"
#include "Resource.h"

#include "CTransform.h"
#include "CMesh.h"
#include "CCamera.h"
#include "CMaterial.h"

#include "RMesh.h"

#include "Dependecies/mmgr/mmgr.h"

GameObject::GameObject(const char* name) : name(name)
{
	AddComponent(new CTransform(this));
}

GameObject::GameObject(GameObject* parent = nullptr, const char* name = "Object") : parent(parent), name(name)
{
	AddComponent(new CTransform(this));
	
	if(parent != nullptr)
		parent->AddChild(this);
}

GameObject::~GameObject()
{

}

void GameObject::Update()
{
	UpdateBoundingBoxes();

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

void  GameObject::AddChild(GameObject* gameObject)
{
	gameObject->SetParent(this);
	children.push_back(gameObject);
}

void GameObject::EraseChild(GameObject* gameObject)
{
	for (std::vector<GameObject*>::iterator child = children.begin(); child != children.end(); child++)
	{
		if ((*child) == gameObject)
		{
			children.erase(child);
			break;
		}
	}
}

bool GameObject::IsToBeDestroyed()const
{
	return toBeDestroyed;
}

void GameObject::SetToBeDestroyed()
{
	LOG("%s has been set to be destroyed", name.c_str());
	

	toBeDestroyed = true;
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
			
			
			components.push_back(component);
			
			
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

	component->owner = this;

	return component;
}

Component* GameObject::AddComponent(ComponentType type)
{
	switch (type)
	{
		case ComponentType::MESH:
			AddComponent(new CMesh(this));
			break;
		case ComponentType::MATERIAL:
			AddComponent(new CMaterial(this));
			break;
		case ComponentType::CAMERA:
			AddComponent(new CCamera(this));
			break;
	}

	return nullptr;
}

void GameObject::OnDelete()
{
	for (std::vector<GameObject*>::iterator child = children.begin(); child != children.end(); child++)
	{
		(*child)->OnDelete();
		RELEASE((*child));
	}

	for (std::vector<Component*>::iterator component = components.begin(); component != components.end(); component++)
	{
		(*component)->CleanUp();
		RELEASE((*component));
	}

	children.clear();
	components.clear();

	CleanUp();
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

void GameObject::UpdateBoundingBoxes()
{
	if (HasComponentType(ComponentType::MESH))
	{
		RMesh* mesh = GetComponent<CMesh>()->GetMesh();
		if (mesh != nullptr)
		{
			OBB = mesh->aabb;
			OBB.Transform(transform->GetGlobalTransform());

			AABB.SetNegativeInfinity();
			AABB.Enclose(OBB);
		}
	}
}

void GameObject::Reparent(GameObject* newParent)
{
	if (newParent == nullptr)
	{
		LOG("(ERROR) Reparent target is nullptr");
		return;
	}
	//check if newParent is children
	if (FindChild(newParent))
	{
		LOG("(ERROR) Reparent target is in object's children");
		return;
	}
	
	parent->EraseChild(this);	//current parent delete this child
	SetParent(newParent);		//this parent = newParent
	parent->AddChild(this);		//newParent child-> add this
}

void GameObject::SetParent(GameObject* newParent)
{
	parent = newParent;
	transform->updateTransform = true;
}

bool GameObject::FindChild(GameObject* newParent)
{
	for (std::vector<GameObject*>::iterator child = children.begin(); child != children.end(); child++)
	{
		if ((*child) == newParent)
		{
			return true;
		}
		(*child)->FindChild(newParent);
	}

	return false;
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

	if ( GetComponent<CCamera>() != nullptr)
	{
		GetComponent<CCamera>()->OnUpdateTransform(transform->GetGlobalTransform());
	}

	//call children's on updateTransforms
	std::vector<GameObject*>::iterator child = children.begin();
	for (; child != children.end(); ++child)
	{
		(*child)->UpdatedTransform();
	}
}
#include "Globals.h"
#include "Config.h"
#include "Random.h"
#include "GameObject.h"
#include "Component.h"
#include "Resource.h"

#include "CTransform.h"
#include "CMesh.h"
#include "CCamera.h"
#include "CMaterial.h"
#include "CAnimator.h"

#include "RMesh.h"

//#include "Dependecies/mmgr/mmgr.h"

GameObject::GameObject(const char* name) : name(name)
{
	AddComponent(new CTransform(this));
	uid = Random::GenerateUID();
}

GameObject::GameObject(GameObject* parent = nullptr, const char* name = "Object") : parent(parent), name(name)
{
	AddComponent(new CTransform(this));

	uid = Random::GenerateUID();

	if(parent != nullptr)
		parent->AddChild(this);
}

GameObject::~GameObject()
{

}


void GameObject::Update(float dt)
{
	UpdateBoundingBoxes();

	if (!components.empty())
	{
		std::vector<Component*>::iterator item = components.begin();
		for (; item != components.end(); ++item)
		{
			(*item)->Update(dt);
		}
	}

	//if(parent != nullptr)
		//transform->UpdatedTransform(parent->transform->GetTransform());
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

void GameObject::OnPlay()
{
	std::vector<Component*>::iterator item = components.begin();
	for (; item != components.end(); ++item)
	{
		(*item)->OnPlay();
	}
}

void GameObject::OnStop()
{
	for (std::vector<Component*>::iterator item = components.begin(); item != components.end(); ++item)
	{
		(*item)->OnStop();
	}
}

void GameObject::Serialize(JsonNode* node)
{
	node->AddString("Name", name.c_str());

	node->AddNumber("UID", uid);
	parent == nullptr ? node->AddNumber("Parent UID", 0) : node->AddNumber("Parent UID", parent->GetUid());

	//Components -----------------------
	JsonArray componentsJson = node->InitArray("Components"); //Create array in GO node

	for (std::vector<Component*>::iterator comp = components.begin(); comp != components.end(); comp++)
	{
		JsonNode newComponent = componentsJson.AddNode(); //Add Component object

		newComponent.AddNumber("Type", (double)(*comp)->GetType());
		(*comp)->Serialize(&newComponent);
	}
}

void GameObject::Load(JsonNode* node)
{
	JsonArray componentsArray = node->GetArray("Components");

	for (uint comp = 0; componentsArray.size; comp++)
	{
		JsonNode componentNode = componentsArray.GetNode(comp);
		int typeInt = (int)componentNode.GetString("Type");
		ComponentType type = (ComponentType)typeInt;

		Component* newComponent = nullptr;

		switch (type)
		{
		case ComponentType::TRANSFORM:
			newComponent = AddComponent(ComponentType::TRANSFORM);
			CTransform* cTrans = (CTransform*)newComponent;
			cTrans->Load(&componentNode);
			break;
		case ComponentType::MESH:
			newComponent = AddComponent(ComponentType::MESH);
			CMesh* cTrans = (CMesh*)newComponent;
			cTrans->Load(&componentNode);
			break;
		case ComponentType::MATERIAL:
			newComponent = AddComponent(ComponentType::MATERIAL);
			CMaterial* cTrans = (CMaterial*)newComponent;
			cTrans->Load(&componentNode);
			break;
		case ComponentType::CAMERA:
			newComponent = AddComponent(ComponentType::CAMERA);
			CCamera* cTrans = (CCamera*)newComponent;
			cTrans->Load(&componentNode);
			break;
		case ComponentType::ANIMATOR:
			newComponent = AddComponent(ComponentType::ANIMATOR);
			CAnimator* cTrans = (CAnimator*)newComponent;
			cTrans->Load(&componentNode);
			break;
		}
	}
}

void GameObject::AddChild(GameObject* gameObject)
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

		case ComponentType::ANIMATOR:
			components.push_back(component);
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
	if (HasChild(newParent))
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

bool GameObject::HasChild(GameObject* newParent)
{
	for (std::vector<GameObject*>::iterator child = children.begin(); child != children.end(); child++)
	{
		if ((*child) == newParent)
		{
			return true;
		}
		(*child)->HasChild(newParent);
	}

	return false;
}

void GameObject::GetChildByName(const char* name, GameObject** recipient)const
{
	for (std::vector<GameObject*>::const_iterator child = children.begin(); child != children.end(); child++)
	{
		if (strcmp(name,(*child)->GetName()) == 0)
		{
			*recipient = (*child);
			return;
		}

		(*child)->GetChildByName(name,recipient);
	}
}

GameObject* GameObject::GetOwnChildByName(const char* name)const
{
	for (std::vector<GameObject*>::const_iterator child = children.begin(); child != children.end(); child++)
	{
		if (strcmp(name, (*child)->GetName()) == 0)
		{
			return (*child);
		}
	}

	return nullptr;
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
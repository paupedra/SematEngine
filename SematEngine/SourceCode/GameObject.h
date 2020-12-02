#ifndef __GAMEOBJECT__
#define __GAMEOBJECT__

#include <string>
#include <vector>

#include "Component.h"
enum class ComponentType;
class CTransform;
class CMesh;
class CMaterial;

class GameObject
{
public:
	GameObject(const char* name);
	GameObject(GameObject* parent, const char* name);
	~GameObject();

	void Update();
	void CleanUp();

	Component* AddComponent(Component* component);

	void Enable();
	void Disable();

	bool IsActive();
	const char* GetName();
	void SetName(const char* name);
	std::vector<Component*> GetComponents()const;

	void DeleteComponentType(ComponentType type);
	bool HasComponentType(ComponentType type);
	void UpdatedTransform();

	template<typename CTemplate>
	const CTemplate* GetComponent() const
	{
		ComponentType type = CTemplate::GetType();
		for (int i = 0; i < components.size(); i++)
		{
			if (type == components[i]->GetType())
			{
				return ((CTemplate*)components[i]);
			}
		}
		return nullptr;
	}

private:
	bool active = false;
	std::string name;
	std::vector<Component*> components;
	
public:
	CTransform* transform = nullptr;
	std::vector<GameObject*> children;
	GameObject* parent = nullptr;
};

#endif //__GAMEOBJECT__
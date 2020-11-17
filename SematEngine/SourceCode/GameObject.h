#ifndef __GAMEOBJECT__
#define __GAMEOBJECT__

#include <string>
#include <vector>

#include "Component.h"
enum class ComponentType;
class ComponentTransform;
class ComponentMesh;
class ComponentTexture;

class GameObject
{
public:
	GameObject(char* name);
	GameObject(GameObject* parent, char* name);
	~GameObject();

	void Update();
	void CleanUp();

	Component* AddComponent(Component* component);

	void Enable();
	void Disable();

	bool IsActive();
	const char* GetName();
	std::vector<Component*> GetComponents()const;

	void DeleteComponentType(Component::ComponentType type);
	bool HasComponentType(Component::ComponentType type);
	void UpdatedTransform();

	template<typename ComponentTemp>
	const ComponentTemp* GetComponent() const
	{
		Component::ComponentType type = ComponentTemp::GetType();
		for (int i = 0; i < components.size(); i++)
		{
			if (type == components[i]->GetType())
			{
				return ((ComponentTemp*)components[i]);
			}
		}
		return nullptr;
	}

private:
	bool active;
	std::string name;
	std::vector<Component*> components;
	
public:
	ComponentTransform* transform = nullptr;
	std::vector<GameObject*> children;
	GameObject* parent;
};

#endif //__GAMEOBJECT__
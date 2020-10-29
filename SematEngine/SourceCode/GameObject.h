#ifndef __GAMEOBJECT__
#define __GAMEOBJECT__

#include <string>
#include <vector>

class Component;
enum  ComponentType;
class ComponentTransform;
class ComponentMesh;

class GameObject
{
public:
	GameObject(char* name);
	GameObject(GameObject* parent, char* name);
	~GameObject();

	void Update();

	Component* AddComponent(Component* component);

	void Enable();
	void Disable();

	bool IsActive();
	const char* GetName();
	std::vector<Component*> GetComponents()const;
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
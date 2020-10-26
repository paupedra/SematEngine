#ifndef __GAMEOBJECT__
#define __GAMEOBJECT__

#include <string>
#include <vector>

class Component;
class ComponentTransform;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();

	//Component* CreateComponent();

	void Enable();
	void Disable();

	bool IsActive();
	const char* GetName();

private:
	bool active;
	std::string name;
	std::vector<Component*> components;

public:
	ComponentTransform* transform;
};

#endif //__GAMEOBJECT__
#ifndef __GAMEOBJECT__
#define __GAMEOBJECT__

#include <string>
#include <vector>

class Component;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();

	void Enable();
	void Disable();

	bool IsActive();
	const char* GetName();

private:
	bool active;
	std::string name;
	std::vector<Component*> components;
};

#endif //__GAMEOBJECT__
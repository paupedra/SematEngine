#ifndef __COMPONENT__
#define __COMPONENT__

class GameObject;

enum ComponentType
{
	TRANSFORM,
	MESH,
	TEXTURE,
	EMPTY
};

class Component
{
public:
	Component(ComponentType type);
	Component(ComponentType type,GameObject* owner);
	~Component();

	virtual void Update() = 0;
	virtual void DrawInspector() = 0;
	void Enable();
	void Disable();

	bool IsActive();
	ComponentType GetType();
	GameObject* GetOwner();

public:
	bool active = true;
	ComponentType type = EMPTY;
	GameObject* owner = nullptr;
};
#endif //__COMPONENT__
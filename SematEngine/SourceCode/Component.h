#ifndef __COMPONENT__
#define __COMPONENT__

class GameObject;

enum type
{
	TRANSFORM,
	MESH,
	EMPTY
};

class Component
{
public:
	Component();
	~Component();

	virtual void Update() = 0;

	void Enable();
	void Disable();

	bool IsActive();
	type GetType();


private:
	bool active;
	type type;
	GameObject* owner;
};
#endif //__COMPONENT__
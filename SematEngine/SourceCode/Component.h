#ifndef __COMPONENT__
#define __COMPONENT__

class GameObject;



class Component
{
public:
	enum class ComponentType
	{
		TRANSFORM,
		MESH,
		TEXTURE,
		EMPTY
	};

public:
	Component(ComponentType type);
	Component(ComponentType type,GameObject* owner);
	~Component();

	virtual void Update() = 0;
	virtual void CleanUp() = 0;
	virtual void DrawInspector() = 0;
	void Enable();
	void Disable();

	bool IsActive();
	inline ComponentType GetType() const { return type; };
	GameObject* GetOwner();

public:
	bool active = true;
	ComponentType type = ComponentType::EMPTY;
	GameObject* owner = nullptr;
};
#endif //__COMPONENT__
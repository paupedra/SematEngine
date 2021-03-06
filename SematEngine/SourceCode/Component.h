#ifndef __COMPONENT__
#define __COMPONENT__

class GameObject;
class JsonNode;

enum class ComponentType
{
	TRANSFORM,
	MESH,
	MATERIAL,
	CAMERA,
	ANIMATOR,
	EMPTY
};

class Component
{
public:
	Component(ComponentType type);
	Component(ComponentType type,GameObject* owner);
	~Component();

	virtual void Update(float dt) = 0;
	virtual void CleanUp() = 0;
	void Enable();
	void Disable();

	virtual void OnPlay() = 0;
	virtual void OnStop() = 0;

	virtual void Serialize(JsonNode* node) = 0;
	virtual void Load(JsonNode* node) = 0;

	bool IsActive();
	inline ComponentType GetType() const { return type; };
	GameObject* GetOwner();

public:
	bool active = true;
	ComponentType type = ComponentType::EMPTY;
	GameObject* owner = nullptr;
};
#endif //__COMPONENT__

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

	void Update();

	void Enable();
	void Disable();

	bool IsActive();
	type GetType;


private:
	bool active;
	type type;
	GameObject* owner;
};

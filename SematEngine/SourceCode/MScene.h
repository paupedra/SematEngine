#include "Module.h"
#include <vector>

class Primitive;
class GameObject;
class JsonNode;
class Component;

class MScene : public Module
{
public:
	MScene(bool start_enabled = true);
	~MScene();

	bool Start();
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;
	bool Save(JsonNode* config) override;

	void SaveScene();
	uint SaveSceneNode(JsonNode* config, std::vector<GameObject*> gameObjects);
	void SaveSceneComponent(JsonNode* node, Component* component);

	GameObject* CreateGameObject(char* name, GameObject* parent= nullptr, bool isRoot = false);
	void SetSelectedObject(GameObject* gameObject);
	std::vector<GameObject*>::iterator FindGameObject(GameObject* gameObject);

	void PrepareToDestroyGameObject(GameObject* gameObject);
	void SetToDestroyGameObject(GameObject* gameObject);
	void EraseGameObject(std::vector<GameObject*>::iterator gameObject);

	void AddGameObject(GameObject* gameObject);

	uint GetNameRepeats(const char* name); //Return how many times this name is repeated on scene

public:

	std::vector<GameObject*> gameObjects;
	GameObject* rootObject = nullptr;
	GameObject* selectedObject = nullptr;
};

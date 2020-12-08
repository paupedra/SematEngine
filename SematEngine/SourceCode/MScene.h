#include "Module.h"
#include <vector>

class Primitive;
class GameObject;
class ConfigNode;
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
	bool Save(ConfigNode* config) override;

	void SaveScene();
	uint SaveSceneNode(ConfigNode* config, std::vector<GameObject*> gameObjects);
	void SaveSceneComponent(ConfigNode* node, Component* component);

	GameObject* CreateGameObject(char* name,char* meshPath = "", char* texturePath = "", bool isRoot = false);
	void SetSelectedObject(GameObject* gameObject);
	std::vector<GameObject*>::iterator FindGameObject(GameObject* gameObject);

	void PrepareToDestroyGameObject(GameObject* gameObject);
	void SetToDestroyGameObject(GameObject* gameObject);
	void EraseGameObject(std::vector<GameObject*>::iterator gameObject);

	

public:

	std::vector<GameObject*> gameObjects;
	GameObject* rootObject = nullptr;
	GameObject* selectedObject = nullptr;
};

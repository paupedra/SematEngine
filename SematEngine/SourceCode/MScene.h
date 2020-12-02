#include "Module.h"
#include <vector>

class Primitive;
class GameObject;
class ConfigNode;

class MScene : public Module
{
public:
	MScene(bool start_enabled = true);
	~MScene();

	bool Start();
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool Save(ConfigNode* config) override;
	void SaveScene();

	GameObject* CreateGameObject(char* name,char* meshPath = "", char* texturePath = "", bool isRoot = false);
	void SetSelectedObject(GameObject* object);

	//void DeleteGameObject();

	bool CleanUp();

public:

	std::vector<GameObject*> gameObjects;
	GameObject* rootObject = nullptr;
	GameObject* selectedObject = nullptr;
};

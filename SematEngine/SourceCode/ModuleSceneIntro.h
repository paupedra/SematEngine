#include "Module.h"
#include "Globals.h"
#include <vector>

class Primitive;
class GameObject;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;

	void CreateGameObject(char* name,char* meshPath, char* texturePath);
	void SetSelectedObject(GameObject* object);

	bool CleanUp();

public:

	std::vector<GameObject*> gameObjects;
	GameObject* selectedObject = nullptr;
};

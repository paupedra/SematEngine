#pragma once
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

	bool CleanUp();

private:

	std::vector<GameObject*> gameObjects;

};

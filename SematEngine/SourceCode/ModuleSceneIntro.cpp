#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
//#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "Primitive.h"
#include "GameObject.h"

#include "Dependecies/imgui/imgui.h"
#include "Dependecies/mmgr/mmgr.h"

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(vec3(0, 1, 0));
	p.axis = true;
	p.Render();

	//Update GameObjects in scene
	std::vector<GameObject*>::iterator item = gameObjects.begin();
	for (; item != gameObjects.end(); ++item)
	{
		(*item)->Update();
	}

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

GameObject* ModuleSceneIntro::CreateGameObject()
{
	GameObject* newGameObject = new GameObject();
	gameObjects.push_back(newGameObject);

	return newGameObject;
}
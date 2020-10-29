#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
//#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "Primitive.h"

#include "Component.h"
#include "GameObject.h"

#include "I_Mesh.h"
#include "ComponentMesh.h"



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

	CreateGameObject("Warrior","Assets/Mesh/warrior/warrior.FBX");
	CreateGameObject("BakerHouse","Assets/Mesh/BakerHouse/BakerHouse.fbx");

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

GameObject* ModuleSceneIntro::CreateGameObject(char* name, char* meshPath = "")
{
	GameObject* newGameObject = new GameObject(nullptr,name);
	gameObjects.push_back(newGameObject);

	if (meshPath != "")
	{
		std::vector<Mesh*> meshes = Importer::MeshImp::Import(meshPath);

		if (meshes.size() == 1)
		{
			ComponentMesh* newComp = new ComponentMesh(newGameObject, meshPath, meshes.front());
			newGameObject->AddComponent((Component*)newComp);
		}
		else
		{
			std::vector<Mesh*>::iterator item = meshes.begin();
			for (; item != meshes.end(); ++item)
			{
				GameObject* childGameObject = new GameObject(newGameObject,"MeshObject");
				ComponentMesh* newComp = new ComponentMesh(childGameObject, meshPath, (*item));

				childGameObject->AddComponent((Component*)newComp);
				newGameObject->children.push_back(childGameObject);

				//gameObjects.push_back(childGameObject);
			}
		}
	}

	return newGameObject;
}

void ModuleSceneIntro::SetSelectedObject(GameObject* object)
{
	if (object != nullptr)
	{
		selectedObject = object;
		LOG("Object selected: %s", selectedObject->GetName());
	}
}
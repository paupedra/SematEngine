#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleCamera3D.h"
#include "Primitive.h"
#include "ModuleInput.h"

#include "Component.h"
#include "GameObject.h"

#include "I_Mesh.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "I_Texture.h"

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

	//Loading Baker House
	CreateGameObject("BakerHouse","Assets/Mesh/BakerHouse/BakerHouse.fbx", "Assets/Mesh/BakerHouse/BakerHouse.png");
	
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	std::vector<GameObject*>::iterator item = gameObjects.begin();
	for (; item != gameObjects.end(); ++item)
	{
		(*item)->CleanUp();
		delete (*item);
	}

	gameObjects.clear();

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
		(*item)->Update();

	if(App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		App->wantToSave = true;

	}

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::CreateGameObject(char* name, char* meshPath = "",char* texturePath = "")
{
	GameObject* newGameObject = nullptr;
	if (meshPath != "")
	{
		std::vector<Mesh*> meshes = Importer::MeshImporter::Import(meshPath);

		if (meshes.size() == 0)
		{
			LOG("(ERROR) No meshes found in %s", meshPath);
			return;
		}

		newGameObject = new GameObject(nullptr, name);
		gameObjects.push_back(newGameObject);

		if (meshes.size() == 1)
		{
			newGameObject->AddComponent(new ComponentMesh(newGameObject, meshPath, meshes.front()));
			if(texturePath != "")
				newGameObject->AddComponent(new ComponentTexture(gameObjects.back(), texturePath, Importer::TextureImp::Import(texturePath)));
		}
		else
		{
			std::vector<Mesh*>::iterator item = meshes.begin();
			for (; item != meshes.end(); ++item)
			{
				GameObject* childGameObject = new GameObject(newGameObject,"MeshObject");
				ComponentMesh* newComp = new ComponentMesh(childGameObject, meshPath, (*item));

				childGameObject->AddComponent((Component*)newComp);
				
				if (texturePath != "")
					childGameObject->AddComponent(new ComponentTexture(gameObjects.back(), texturePath, Importer::TextureImp::Import(texturePath)));

				newGameObject->children.push_back(childGameObject);
			}
		}
	}
}

void ModuleSceneIntro::SetSelectedObject(GameObject* object)
{
	if (object != nullptr)
	{
		selectedObject = object;
		LOG("Object selected: %s", selectedObject->GetName());
	}
}
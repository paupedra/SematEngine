#include "Globals.h"
#include "Application.h"
#include "Component.h"
#include "GameObject.h"
#include "Config.h"

#include "Primitive.h"

#include "MScene.h"
#include "MCamera3D.h"
#include "MInput.h"
#include "MRenderer3D.h"
#include "MFileSystem.h"

#include "CMesh.h"
#include "CMaterial.h"
#include "CTransform.h"

#include "IMesh.h"
#include "ITexture.h"
#include "IScene.h"

#include "Dependecies/imgui/imgui.h"
#include "Dependecies/mmgr/mmgr.h"

MScene::MScene(bool start_enabled) : Module(start_enabled)
{}

MScene::~MScene()
{}

// Load assets
bool MScene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	rootObject = CreateGameObject("rootObject","","",true);

	//Loading Baker House
	//CreateGameObject("BakerHouse","Assets/Mesh/BakerHouse/BakerHouse.fbx", "Assets/Mesh/BakerHouse/BakerHouse.png");

	//CreateGameObject("Street", "Assets/Mesh/street/Street environment_V01.FBX");

	Importer::SceneImporter::Import("Assets/Mesh/street/Street environment_V01.FBX");
	//Importer::SceneImporter::Import("Assets/Mesh/BakerHouse/BakerHouse.fbx");
	
	return ret;
}

// Load assets
bool MScene::CleanUp()
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
update_status MScene::Update(float dt)
{
	//Plane p(vec3(0, 1, 0));
	//p.axis = true;
	//p.Render();

	App->renderer3D->DrawScenePlane(200);

	//LOG("before update GOs");
	//Update GameObjects in scene
	std::vector<GameObject*>::iterator item = gameObjects.begin();
	for (; item != gameObjects.end(); ++item)
	{
		(*item)->Update();
	}
	//LOG("after update GOs");

	return UPDATE_CONTINUE;
}

update_status MScene::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool MScene::Save(ConfigNode* config)
{
	bool ret = true;
	LOG("Saving scene");

	
	
	return ret;
}

void MScene::SaveScene()
{
	ConfigNode* sceneNode;

	uint64 id = Importer::SceneImporter::SaveScene(sceneNode, gameObjects);

	std::string path = "Library/Scenes/";
	std::string idString = std::to_string(id);
	path += idString + ".scene";

	//Create and save scene meta file
	char* buffer;
	uint size = sceneNode->Serialize(&buffer); //Saves for now
	App->fileSystem->Save(path.c_str(), buffer, size);
}

GameObject* MScene::CreateGameObject(char* name, char* meshPath,char* texturePath, bool isRoot)
{
	GameObject* newGameObject = nullptr;

	if (isRoot)
	{
		newGameObject = new GameObject(nullptr, name);
		return newGameObject;
	}

	if (meshPath != "")
	{
		std::vector<RMesh*> meshes = Importer::MeshImporter::Import(meshPath);

		if (meshes.size() == 0)
		{
			LOG("(ERROR) No meshes found in %s", meshPath);
			return newGameObject;
		}

		newGameObject = new GameObject(rootObject, name);
		gameObjects.push_back(newGameObject);
		rootObject->children.push_back(newGameObject);

		if (meshes.size() == 1)
		{
			newGameObject->AddComponent(new CMesh(newGameObject, meshPath, meshes.front()));
			if(texturePath != "")
				newGameObject->AddComponent(new CMaterial(gameObjects.back(), texturePath, Importer::TextureImp::Import(texturePath)));
		}
		else
		{
			std::vector<RMesh*>::iterator item = meshes.begin();
			for (; item != meshes.end(); ++item)
			{
				GameObject* childGameObject = new GameObject(newGameObject,"MeshObject");
				CMesh* newComp = new CMesh(childGameObject, meshPath, (*item));

				childGameObject->AddComponent((Component*)newComp);
				
				if (texturePath != "")
					childGameObject->AddComponent(new CMaterial(gameObjects.back(), texturePath, Importer::TextureImp::Import(texturePath)));

				gameObjects.push_back(childGameObject);
				newGameObject->children.push_back(childGameObject);
			}
		}
	}

	return newGameObject;
}

void MScene::SetSelectedObject(GameObject* object)
{
	if (object != nullptr)
	{
		selectedObject = object;
		LOG("Object selected: %s", selectedObject->GetName());
	}
}
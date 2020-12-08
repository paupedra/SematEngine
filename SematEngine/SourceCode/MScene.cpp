#include "Globals.h"
#include "Application.h"
#include "Component.h"
#include "GameObject.h"
#include "Config.h"

#include "MScene.h"
#include "MCamera3D.h"
#include "MInput.h"
#include "MRenderer3D.h"
#include "MFileSystem.h"

#include "CMesh.h"
#include "CMaterial.h"
#include "CTransform.h"
#include "CCamera.h"

#include "IMesh.h"
#include "ITexture.h"
#include "IScene.h"

#include "RMaterial.h"
#include "RMesh.h"

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

	App->camera->Move(float3(1.0f, 1.0f, 0.0f));
	//App->camera->LookAt(float3(0, 0, 0));

	rootObject = CreateGameObject("rootObject","","",true);

	GameObject* obj = new GameObject(rootObject,"camera bro");
	rootObject->children.push_back(obj);
	obj->AddComponent(new CCamera(obj));
	gameObjects.push_back(obj);

	Importer::SceneImporter::Import("Assets/Mesh/Street environment_V01.FBX");
	
	return ret;
}

// Update
update_status MScene::Update(float dt)
{
	ImGui::ShowDemoWindow();

	App->renderer3D->DrawScenePlane(200);

	//Update GameObjects in scene
	//std::vector<GameObject*>::iterator item = gameObjects.begin();
	for (int i = 0; i < gameObjects.size(); ++i)
	{
		if (gameObjects[i]->IsToBeDestroyed())
		{
			LOG("%s was destroyed!", gameObjects[i]->GetName());
			gameObjects[i]->CleanUp(); 
			RELEASE(gameObjects[i]);

			gameObjects.erase(gameObjects.begin() + i);

			i--;
			continue;
		}

		gameObjects[i]->Update();
	}

	return UPDATE_CONTINUE;
}

update_status MScene::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool MScene::CleanUp()
{
	LOG("Unloading Intro scene");

	std::vector<GameObject*>::iterator item = gameObjects.begin();
	for (; item != gameObjects.end(); ++item)
	{
		(*item)->CleanUp();
		RELEASE(*item);
		
	}

	gameObjects.clear();

	return true;
}

bool MScene::Save(ConfigNode* config)
{
	bool ret = true;
	LOG("Saving scene");

	return ret;
}

void MScene::SaveScene()
{
	LOG("Started saving current scene");
	ConfigNode sceneNode;

	uint64 id = SaveSceneNode(&sceneNode, gameObjects);

	std::string path = "Library/Scenes/";
	std::string idString = std::to_string(id);
	path += idString + ".scene";

	//Create and save scene meta file
	char* buffer;
	uint size = sceneNode.Serialize(&buffer); //Saves for now
	App->fileSystem->Save(path.c_str(), buffer, size);
}

uint MScene::SaveSceneNode(ConfigNode* config, std::vector<GameObject*> gameObjects)
{
	JSON_Value* currentNode;
	config->rootNode = json_value_init_object(); //root
	config->node = json_value_get_object(config->rootNode);

	//gameObjects -----------------------
	ConfigArray gameObjectsJson = config->InitArray("GameObjects");

	std::vector<GameObject*>::iterator item = gameObjects.begin();
	for (; item != gameObjects.end(); item++)
	{
		ConfigNode newObject = gameObjectsJson.AddNode(); //Create object in GOs array

		//Components -----------------------
		ConfigArray components = newObject.InitArray("Components"); //Create array in GO node
		std::vector<Component*> comps = (*item)->GetComponents();
		for (int i = 0; i < comps.size(); i++)
		{
			ConfigNode newComponent = components.AddNode(); //Add Component object

			newComponent.AddNumber("Type", (double)comps[i]->GetType());
			SaveSceneComponent(&newComponent, comps[i]);
		}
	}

	return 5467;
}

void MScene::SaveSceneComponent(ConfigNode* node, Component* component)
{
	CMesh* cMesh = nullptr;
	CMaterial* cMaterial = nullptr;
	CTransform* cTransform = nullptr;

	switch (component->GetType())
	{
		case ComponentType::TRANSFORM:

			cTransform = (CTransform*)component;
			cTransform->OnSave(node);

			break;

		case ComponentType::MESH:

			cMesh = (CMesh*)component;
			node->AddNumber("UID", cMesh->GetMesh()->GetUID());
			cMesh->OnSave(node);

			break;

		case ComponentType::MATERIAL:

			cMaterial = (CMaterial*)component;
			node->AddNumber("UID", cMaterial->GetMaterial()->GetUID());
			cMaterial->OnSave(node);

			break;
	}
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
				newGameObject->AddComponent(new CMaterial(gameObjects.back(), texturePath, new RMaterial(Importer::TextureImp::Import(texturePath))));
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
					childGameObject->AddComponent(new CMaterial(gameObjects.back(), texturePath, new RMaterial(Importer::TextureImp::Import(texturePath))));

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
	else
	{
		selectedObject = nullptr;
	}
}

std::vector<GameObject*>::iterator MScene::FindGameObject(GameObject* gameObject)
{

	return gameObjects.begin();
}

void MScene::PrepareToDestroyGameObject(GameObject* gameObject)
{
	gameObject->parent->EraseChild(gameObject);
	SetToDestroyGameObject(gameObject);
}

void MScene::SetToDestroyGameObject(GameObject* gameObject)
{
	gameObject->SetToBeDestroyed();

	for (std::vector<GameObject*>::iterator child = gameObject->children.begin(); child != gameObject->children.end(); child++)
	{
		SetToDestroyGameObject((*child));
	}
}

void MScene::EraseGameObject(std::vector<GameObject*>::iterator gameObject)
{
	gameObjects.erase(gameObject);
}
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
#include "MResourcemanager.h"

#include "CMesh.h"
#include "CMaterial.h"
#include "CTransform.h"
#include "CCamera.h"

#include "IMesh.h"
#include "ITexture.h"
#include "IScene.h"

#include "RMaterial.h"
#include "RMesh.h"

#include "Random.h"

#include "Dependecies/imgui/imgui.h"
#include "Dependecies/mmgr/mmgr.h"

MScene::MScene(bool start_enabled) : Module(start_enabled)
{
	rootObject = CreateGameObject("rootObject", nullptr, true);
}

MScene::~MScene()
{}

// Load assets
bool MScene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(float3(1.0f, 1.0f, 0.0f));
	//App->camera->LookAt(float3(0, 0, 0));

	//GameObject* obj = new GameObject(rootObject,"camera bro");
	//rootObject->children.push_back(obj);
	//obj->AddComponent(new CCamera(obj));
	//AddGameObject(obj);

	//Importer::SceneImporter::Import("Assets/Mesh/Street environment_V01.FBX");

	//App->resourceManager->ImportFile("Assets/Mesh/Street environment_V01.FBX", ResourceType::scene);
	
	return ret;
}

// Update
update_status MScene::Update(float dt)
{
	//ImGui::ShowDemoWindow();

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
	for (; item != gameObjects.end(); item++)
	{
		(*item)->CleanUp();
		RELEASE(*item);
		
	}

	gameObjects.clear();

	return true;
}

bool MScene::Save(JsonNode* config)
{
	bool ret = true;
	LOG("Saving scene");

	return ret;
}

void MScene::SaveScene()
{
	LOG("Started saving current scene");
	JsonNode sceneNode;

	uint64 id = SaveSceneNode(&sceneNode, gameObjects);

	std::string path = "Library/Scenes/";
	std::string idString = std::to_string(id);
	path += idString + ".scene";

	//Create and save scene meta file
	char* buffer;
	uint size = sceneNode.Serialize(&buffer); //Saves for now
	App->fileSystem->Save(path.c_str(), buffer, size);
	delete[] buffer;
}

uint MScene::SaveSceneNode(JsonNode* config, std::vector<GameObject*> gameObjects)
{

	//gameObjects -----------------------
	JsonArray gameObjectsJson = config->InitArray("GameObjects");

	std::vector<GameObject*>::iterator item = gameObjects.begin();
	for (; item != gameObjects.end(); item++)
	{
		JsonNode newObject = gameObjectsJson.AddNode(); //Create object in GOs array

		newObject.AddString("Name", (*item)->GetName());

		//Components -----------------------
		JsonArray components = newObject.InitArray("Components"); //Create array in GO node
		std::vector<Component*> comps = (*item)->GetComponents();
		for (int i = 0; i < comps.size(); i++)
		{
			JsonNode newComponent = components.AddNode(); //Add Component object

			newComponent.AddNumber("Type", (double)comps[i]->GetType());
			SaveSceneComponent(&newComponent, comps[i]);
		}
	}

	return 5467;
}

void MScene::SaveSceneComponent(JsonNode* node, Component* component)
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

GameObject* MScene::CreateGameObject(char* name, GameObject* parent, bool isRoot)
{
	GameObject* newGameObject;
	uint repeats = GetNameRepeats(name);

	if (isRoot)
	{
		newGameObject = new GameObject(nullptr, "Root Object");
	}
	else
	{

		std::string finalName = name;
		if(repeats >0)
			finalName += "(" + std::to_string(repeats) + ")";

		newGameObject = new GameObject(parent,finalName.c_str());
	}

	gameObjects.push_back(newGameObject);

	return newGameObject;
}

uint MScene::GetNameRepeats(const char* name)
{
	uint repeats = 0;
	for (std::vector<GameObject*>::iterator item = gameObjects.begin(); item != gameObjects.end(); item++)
	{
		//get rid of the (n)
		std::string str2;
		std::string str = (*item)->GetName();
		if (str.find("(") == std::string::npos) 
		{
			str2 = str;
		}
		else
		{
			str2 = str.substr(0, str.find_last_of("("));
		}

		if (strcmp(name, str2.c_str()) == 0)
			repeats += 1;
	}
	return repeats;
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
	if (gameObject == nullptr)
		return;

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

void MScene::AddGameObject(GameObject* gameObject)
{
	gameObjects.push_back(gameObject);
}
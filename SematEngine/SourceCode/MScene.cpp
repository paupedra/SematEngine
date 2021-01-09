#include "Globals.h"
#include "Application.h"
#include "Component.h"
#include "GameObject.h"
#include "Config.h"
#include "Random.h"

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
#include "CAnimator.h"

#include "IMesh.h"
#include "ITexture.h"
#include "IScene.h"

#include "RMaterial.h"
#include "RMesh.h"

#include "Random.h"

#include "Dependecies/imgui/imgui.h"
//#include "Dependecies/mmgr/mmgr.h"

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

	FindSavedScenes();
	
	return ret;
}

// Update
updateStatus MScene::Update(float dt)
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

		gameObjects[i]->Update(App->GetPlayDt());
	}

	return UPDATE_CONTINUE;
}

updateStatus MScene::PostUpdate(float dt)
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
	gameObjects.shrink_to_fit();

	selectedObject = nullptr;
	rootObject = nullptr;

	if (playSavedScene != 0 && App->IsExiting())
	{
		std::string file = "Assets/Scenes/" + std::to_string(playSavedScene) + ".scene";
		App->fileSystem->Remove(file.c_str());
	}

	return true;
}

bool MScene::Save(JsonNode* config)
{
	bool ret = true;
	LOG("Saving scene");
	//SaveScene();

	return ret;
}

void MScene::OnPlay()
{
	playSavedScene = SaveScene(true);

	for (std::vector<GameObject*>::iterator item = gameObjects.begin(); item != gameObjects.end(); item++)
	{
		(*item)->OnPlay();
	}
}

void MScene::OnStop()
{
	//load scene
	LoadScene(playSavedScene);

	std::string file = "Assets/Scenes/" + std::to_string(playSavedScene) + ".scene";
	App->fileSystem->Remove(file.c_str());
	playSavedScene = 0;

	for (std::vector<GameObject*>::iterator item = gameObjects.begin(); item != gameObjects.end(); item++)
	{
		(*item)->OnStop();
	}
}

uint MScene::SaveScene(bool isPlay)
{
	LOG("Started saving current scene");
	JsonNode sceneNode;

	uint64 id = SerializeScene(&sceneNode);

	std::string path = "Assets/Scenes/" + std::to_string(id) + ".scene";

	//Create and save scene meta file
	char* buffer = nullptr;
	uint size = sceneNode.Serialize(&buffer); //Saves for now
	App->fileSystem->Save(path.c_str(), buffer, size);
	RELEASE_ARRAY(buffer);

	if(isPlay)
	{
		playSavedScene = id;
	}
	else
	{
		savedScenes.push_back(id);
	}
	return id;
}

uint MScene::SerializeScene(JsonNode* node)
{
	JsonArray gameObjectsJson = node->InitArray("GameObjects");

	for (std::vector<GameObject*>::iterator item = gameObjects.begin(); item != gameObjects.end(); item++)
	{
		JsonNode newObject = gameObjectsJson.AddNode(); //Create object in GOs array

		(*item)->Serialize(&newObject);
	}

	return Random::GenerateUID();
}

void MScene::LoadScene(uint sceneUid)
{
	CleanUp();

	//load file
	std::string file = "Assets/Scenes/" + std::to_string(sceneUid) + ".scene";

	char* buffer = nullptr;
	App->fileSystem->Load(file.c_str(), &buffer);
	JsonNode sceneNode(buffer);
	RELEASE_ARRAY(buffer);

	std::multimap<UID, GameObject*> parentObjectsMap; //Used to find parents later
	std::map<UID, GameObject*> gameObjectsMap; //Used to find parents later
	JsonArray gameObjectsArray = sceneNode.GetArray("GameObjects");

	for (uint i = 0; i < gameObjectsArray.size; i++)
	{
		JsonNode gameObjectNode = gameObjectsArray.GetNode(i);
		GameObject* newGameObject = new GameObject(gameObjectNode.GetString("Name"));
		newGameObject->SetUid(gameObjectNode.GetNumber("UID"));

		newGameObject->Load(&gameObjectNode);

		parentObjectsMap.emplace(gameObjectNode.GetNumber("Parent UID"), newGameObject);
		gameObjectsMap.emplace(newGameObject->GetUid(), newGameObject);
	}

	for (auto i = parentObjectsMap.begin(); i != parentObjectsMap.end(); i++)
	{
		gameObjects.push_back(i->second);

		std::map<UID, GameObject*>::const_iterator it = gameObjectsMap.find(i->first); //Parent
		if (it != gameObjectsMap.end())
		{
			i->second->SetParent(it->second);
		}

		//gameObjects.push_back(i->second);	//add to scene
	}

	std::map<UID, GameObject*>::const_iterator it = parentObjectsMap.find(0); //Parent
	if (it != parentObjectsMap.end())
	{
		rootObject = it->second;
	}
}

void MScene::FindSavedScenes()
{
	std::vector<std::string> files;
	std::vector<std::string> dirs;
	App->fileSystem->DiscoverFiles("Assets/Scenes/", files,dirs);

	for (auto i = files.begin(); i != files.end(); i++)
	{
		std::string file;
		App->fileSystem->SplitFilePath(i->c_str(),nullptr,&file);
		savedScenes.push_back(std::atoi(file.c_str()));
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

void MScene::SetModifiedAnimation(CAnimator* animator)
{
	if (animator != nullptr)
	{
		if (modifiedAnimation != nullptr)
			modifiedAnimation->SetmodifiedAnimation(false);

		modifiedAnimation = animator;
		modifiedAnimation->SetmodifiedAnimation(true);
	}
}
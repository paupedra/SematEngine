#include "Application.h"
#include "Globals.h"
#include "Resource.h"
#include "GameObject.h"

#include "MFileSystem.h"
#include "MScene.h"

#include "CMaterial.h"

#include "RMaterial.h"

#include "IScene.h"
#include "ITexture.h"

#include "MResourceManager.h"

MResourceManager::MResourceManager(bool start_enabled) : Module(start_enabled)
{

}

// Destructor
MResourceManager::~MResourceManager()
{

}

bool MResourceManager::Init()
{
	return true;
}

bool MResourceManager::CleanUp()
{
	LOG("Cleaning Resource System");
	return true;;
}

void MResourceManager::Import(const char* path)
{
	std::string str = App->fileSystem->NormalizePath(path);

	std::string str2 = str.substr(str.find("Assets"));

	LOG("File is %s", str2.c_str());

	if (strstr(str2.c_str(), ".fbx") != nullptr || strstr(".FBX",str2.c_str()) != nullptr )
	{
		Importer::SceneImporter::Import(str2.c_str());
	}

	if (strstr(str2.c_str(), ".png") != nullptr || strstr(".PNG", str2.c_str()) != nullptr)
	{
		App->scene->selectedObject->GetComponent<CMaterial>()->GetMaterial()->SetTexture(Importer::TextureImp::Import(str2.c_str()));
	}
}

uint MResourceManager::ImportFile(const char* newFileInAssets)
{
	//Get resource type somehow?
	//Create resource
	//Resource* resource = CreateNewResource(newFileInAssets);

	//Import in a switch using the ype

	//save it
	
	return 0;
}

Resource* MResourceManager::CreateNewResource(const char* assetsFile, ResourceType type)
{

	return nullptr;
}
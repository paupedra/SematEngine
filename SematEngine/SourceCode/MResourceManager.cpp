#include "Globals.h"
#include "Resource.h"

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
	return true;
}

uint ImportFile(const char* newFileInAssets)
{
	//Get resource type somehow?
	//Create resource
	//Resource* resource = CreateNewResource(newFileInAssets);

	//Import in a switch using the ype

	//save it
	return 0;
}

Resource* CreateNewResource(const char* assetsFile, ResourceType type)
{

	return nullptr;
}
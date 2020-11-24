#include "Globals.h"

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
#ifndef __ModuleResourceManager_H__
#define __ModuleResourceManager_H__

#include <map>
#include "Module.h"
#include "Dependecies/SDL/include/SDL.h"


class Resource;
enum class ResourceType;

class MResourceManager : public Module
{
public:

	MResourceManager(bool start_enabled = true);

	// Destructor
	virtual ~MResourceManager();

	bool Init();
	bool CleanUp();

	uint Find(const char* file_in_assets) const;
	void Import(const char* path);
	uint ImportFile(const char* newFileInAssets);
	uint GenerateNewUID();

	const Resource* RequestResource(uint uid) const;
	Resource* RequestResource(uint uid);
	void ReleaseResource(uint uid);

private:

	Resource* CreateNewResource(const char* assetsFile, ResourceType type);

private:
	std::map<uint, Resource*> resources;


};

#endif // __ModuleResourceManager_H__
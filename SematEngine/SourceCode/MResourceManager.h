#ifndef __ModuleResourceManager_H__
#define __ModuleResourceManager_H__

#include <map>
#include "Module.h"
#include "Dependecies/SDL/include/SDL.h"

class Resource;
enum class ResourceType;
class RMesh;

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
	uint ImportFile(const char* newFileInAssets, ResourceType type);
	uint GenerateNewUID();

	const char* GenerateMeatFile(Resource* resource);

	void ImportScene();

	const Resource* RequestResource(uint uid) const;
	Resource* RequestResource(uint uid);
	void ReleaseResource(uint uid);

	void GenerateMeta();

private:

	Resource* CreateNewResource(const char* assetsFile, ResourceType type); //Create and store info in new resource (UID, path ,libraryFile)

private:
	std::map<UID, Resource*> resources;


};

#endif // __ModuleResourceManager_H__
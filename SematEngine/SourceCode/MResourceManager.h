#ifndef __ModuleResourceManager_H__
#define __ModuleResourceManager_H__

#include <map>
#include "Module.h"
#include "Dependecies/SDL/include/SDL.h"

class Resource;
enum class ResourceType;
class RMesh;
class JsonNode;

class MResourceManager : public Module
{
public:

	MResourceManager(bool start_enabled = true);

	// Destructor
	virtual ~MResourceManager();

	bool Init();
	bool CleanUp();

	uint Find(const char* fileInAssets); //Look for a file's meta data to see if we have loaded it already
	void Import(const char* path);
	uint ImportFile(const char* newFileInAssets, ResourceType type);
	uint GenerateNewUID();

	const char* GenerateMeatFile(Resource* resource, ResourceType type); //Generate meta file that holds UID and other information
	const char* GenerateLibraryFile(Resource* resource,ResourceType type); //Generates and saves the resource's custom file format

	void GenerateTextureMetaFile(Resource* resource,JsonNode node);

	void ImportScene();


	Resource* RequestResource(uint uid);
	void ReleaseResource(uint uid);

	void GenerateMeta();

private:

	Resource* CreateNewResource(const char* assetsFile, ResourceType type); //Create and store info in new resource (UID, path ,libraryFile)

private:
	std::map<UID, Resource*> resources;


};

#endif // __ModuleResourceManager_H__
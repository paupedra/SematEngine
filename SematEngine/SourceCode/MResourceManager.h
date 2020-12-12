#ifndef __ModuleResourceManager_H__
#define __ModuleResourceManager_H__

#include <map>
#include "Module.h"
#include "Dependecies/SDL/include/SDL.h"

class Resource;
struct ResourceData;
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

	bool FindMeta(const char* fileInAssets); //Look if we have a meta file for this assets files
	void Import(const char* path);
	uint ImportFile(const char* newFileInAssets, ResourceType type);

	const char* GenerateMeatFile(Resource* resource); //Generate meta file that holds UID and other information
	std::string GenerateLibraryFile(Resource* resource); //Generates and saves the resource's custom file format

	uint GenerateSceneFile(Resource* resource); //Generate .scene json file. Returns json serialized size

	UID LoadResource(UID uid); //Load resource from meta file into memory. uid of the ResourceData
	void LoadScene(ResourceData resource); //Load scene from library file

	void SaveResource(Resource* resource); //Generates meta file and saves into library with custom file format

	void AddResourceToLibrary(Resource* resource);
	void AddResourceToLibraryFromMeta(const char* file); //Adds resource from meta to resourcesInLibrary map

	Resource* RequestResource(uint uid);
	void ReleaseResource(uint uid);
	ResourceData RequestLibraryResource(uint uid); //Request ResourceData from resourcesInLibrary

private:

	Resource* CreateNewResource(const char* assetsFile, ResourceType type); //Create and store info in new resource (UID, path ,libraryFile)

	void ImportAllFoundAssets(const char* basePath);

	uint GetResourceTypeFromPath();

public:
	std::map<UID, Resource*> resources; //Resource in memory

	std::map<UID, ResourceData> resourcesInLibrary; //Resources not loaded in memory but imported i meta & custom file format
};

#endif // __ModuleResourceManager_H__
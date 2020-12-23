#ifndef __ModuleResourceManager_H__
#define __ModuleResourceManager_H__

#include <map>
#include "Module.h"
#include "Dependecies/SDL/include/SDL.h"

class Resource;
struct ResourceData;
enum class ResourceType;
class RMesh;
class RMaterial;
class JsonNode;
struct Color;

class MResourceManager : public Module
{
public:
	MResourceManager(bool start_enabled = true);

	// Destructor
	virtual ~MResourceManager();

	bool Init();
	bool Start();
	bool CleanUp();

	bool FindMeta(const char* fileInAssets); //Look if we have a meta file for this assets files
	void Import(const char* path);
	uint ImportFileFromAssets(const char* newFileInAssets, ResourceType type);

	uint GetResourceUID(const char* metaFile); //Get the metaFile's uid

	uint ImportModelResource(const char* newFileInAssets, ResourceType type); //Import resources from model (mesh, material, animation)

	uint ImportMaterial(const char* file,uint textureUID,Color color); //Creates RMaterial and saves .material (cff)
	uint ImportAnimationCollection();

	const char* GenerateMetaFile(Resource* resource); //Generate meta file that holds UID and other information
	std::string GenerateLibraryFile(Resource* resource); //Generates and saves the resource's custom file format

	UID LoadResource(UID uid);	//Load resource from meta file into memory. uid of the ResourceData
	void LoadResourceFromMeta(std::string meta);
	UID LoadModelResource(UID uid, ResourceType type); //this includes meshes

	void LoadScene(ResourceData resource); //Load scene from library file
	void LoadMesh(UID uid);
	void LoadAnimation(UID uid);
	RMaterial* LoadMaterial(UID uid);
	void LoadTexture(ResourceData resource);

	void SaveResource(Resource* resource,bool meta= true); //Generates meta file and saves into library with custom file format

	void AddResourceToLibrary(Resource* resource);
	int AddResourceToLibraryFromMeta(const char* file); //Adds resource from meta to resourcesInLibrary map

	Resource* RequestResource(uint uid);
	void ReleaseResource(uint uid);
	ResourceData RequestLibraryResource(uint uid); //Request ResourceData from resourcesInLibrary

	void DereferenceResource(uint uid);

private:

	Resource* CreateNewResource(const char* assetsFile, ResourceType type); //Create and store info in new resource (UID, path ,libraryFile)

	void ImportAllFoundAssets(const char* basePath);

	uint GetResourceTypeFromPath(const char* path);

public:
	std::map<UID, Resource*> resources; //Resource in memory

	std::map<UID, ResourceData> resourcesInLibrary; //Resources not loaded in memory but imported i meta & custom file format
};

#endif // __ModuleResourceManager_H__
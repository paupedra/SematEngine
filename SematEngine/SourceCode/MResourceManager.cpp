#include "Application.h"
#include "Globals.h"
#include "Resource.h"
#include "GameObject.h"
#include "Random.h"
#include "Config.h"

#include "MFileSystem.h"
#include "MScene.h"

#include "CMaterial.h"

#include "RMaterial.h"
#include "RMesh.h"
#include "RTexture.h"
#include "RScene.h"
#include "RModel.h"

#include "IScene.h"
#include "ITexture.h"
#include "IMesh.h"

#include "MResourceManager.h"

#include "Dependecies/mmgr/mmgr.h"

MResourceManager::MResourceManager(bool start_enabled) : Module(start_enabled)
{

}

// Destructor
MResourceManager::~MResourceManager()
{

}

bool MResourceManager::Init()
{
	ImportAllFoundAssets("Assets");
	return true;
}

bool MResourceManager::Start()
{
	LoadResourceFromMeta("Assets/Mesh/Street environment_V01.FBX.meta");
	return true;
}

bool MResourceManager::CleanUp()
{
	App->fileSystem->Remove("Library/");
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
		//App->scene->selectedObject->GetComponent<CMaterial>()->GetMaterial()->SetTexture(Importer::TextureImp::Import(str2.c_str()));
	}
}

uint MResourceManager::ImportFile(const char* newFileInAssets, ResourceType type)
{
	UID ret = 0;

	if (type == ResourceType::none)
	{
		return ret;
	}

	if (FindMeta(newFileInAssets))
	{
		std::string str = newFileInAssets;
		str += ".meta";
		AddResourceToLibraryFromMeta(str.c_str());		//load file in this meta and add it to resourcesInLibrary
		return ret;
	}

	char* fileBuffer;
	uint size = App->fileSystem->Load(newFileInAssets,&fileBuffer);
	Resource* resource = CreateNewResource(newFileInAssets, type);

	switch (type)
	{
		case ResourceType::texture: Importer::TextureImp::Import(fileBuffer, (RTexture*)resource, size); break;
		case ResourceType::scene: Importer::SceneImporter::ImportSceneResource(fileBuffer,(RScene*)resource,size); break;
		case ResourceType::none:
				return ret; 
				break;
	}

	SaveResource(resource); 
	ret = resource->resourceData.UID;
	RELEASE_ARRAY(fileBuffer);
	RELEASE(resource);

	return ret;
}

Resource* MResourceManager::CreateNewResource(const char* assetsFile, ResourceType type)
{
	Resource* ret = nullptr;
	UID uid = Random::GenerateUID();

	switch (type) 
	{
		case ResourceType::texture: ret = (Resource*) new RTexture(uid); break;
		case ResourceType::mesh: ret = (Resource*) new RMesh(uid); break;
		case ResourceType::material: ret = (Resource*) new RMaterial(uid); break;
		case ResourceType::scene: ret = (Resource*) new RScene(uid); break;
	}
	if (ret != nullptr)
	{
		ret->resourceData.assetsFile = assetsFile;
		ret->resourceData.UID = uid;
		ret->resourceData.libraryFile = GenerateLibraryFile(ret);
	}

	return ret;
}

const char* MResourceManager::GenerateMeatFile(Resource* resource)
{
	std::string ret = "default";
	JsonNode node;
	resource->resourceData.Serialize(node);

	std::string path;
	path = resource->resourceData.assetsFile + ".meta";

	char* buffer;
	uint size = node.Serialize(&buffer);
	App->fileSystem->Save(path.c_str(), buffer, size);

	switch (resource->GetType())
	{
		case ResourceType::texture: 
			LOG("Generationg Texture meta file: %s", path.c_str());
			break;
		case ResourceType::scene:
			LOG("Generationg Scene meta file: %s", path.c_str());
			break;
	}

	return ret.c_str();
}

std::string MResourceManager::GenerateLibraryFile(Resource* resource)
{
	std::string path = "";
	char* buffer = "";
	uint size = 1;
	switch (resource->GetType())
	{
		case ResourceType::texture: 
			path = TEXTURES_PATH;
			path += std::to_string(resource->GetUID());
			path += ".tex";
			break;

		case ResourceType::scene:
			path = SCENES_PATH;
			path += std::to_string(resource->GetUID());
			path += ".scene";
			break;
	}
	return path;
}

bool MResourceManager::FindMeta(const char* fileInAssets)
{
	bool ret = false;
	std::string file = fileInAssets;
	file += ".meta";
	ret = App->fileSystem->Exists(file.c_str());
	return ret;
}

UID MResourceManager::LoadResource(UID uid)
{
	uint ret = 0;
	ResourceData resource = RequestLibraryResource(uid);
	
	switch (resource.type)
	{
		case ResourceType::scene: 
			LoadScene(resource);
			break;
		case ResourceType::mesh:
			LoadMesh(uid);
			break;
		case ResourceType::none:
			return ret;
			break;
	}

	return ret;
}

void MResourceManager::LoadResourceFromMeta(std::string meta)
{
	std::map<UID, ResourceData>::iterator item = resourcesInLibrary.begin();
	for (; item != resourcesInLibrary.end(); item++)
	{
		if ( strstr(meta.c_str(),(*item).second.assetsFile.c_str()) != nullptr)
		{
			LoadResource((*item).first);
		}
	}
}

UID MResourceManager::LoadModelResource(UID uid,ResourceType type)
{
	UID ret = 0;

	switch (type)
	{
		case ResourceType::mesh:
			LoadMesh(uid);
			break;
	}
	return ret;
}

void MResourceManager::LoadScene(ResourceData resource)
{
	//Load .scene file and go through all the models and load and add to memory resources
	char* buffer;
	App->fileSystem->Load(resource.libraryFile.c_str(), &buffer);

	JsonNode node(buffer);
	JsonArray modelsJson = node.GetArray("Models");

	std::map<UID, ModelNode> models;

	for (int i = 0; i < modelsJson.size ; i++) //Add All models into a map
	{
		JsonNode newModelJson = modelsJson.GetNode(i);
		ModelNode newNode(0);
		newNode.model.ReadJsonNode(newModelJson);
		models.emplace(newNode.model.uid, newNode);
	}

	ModelNode* root;
	//tree structure the map
	for (std::map<UID, ModelNode>::iterator item = models.begin(); item != models.end(); item++)
	{
		//Add parent to item's parent, Add item to parent's chidlren
		if ((*item).second.model.parentUID != 0)
		{
			ModelNode* parent = &(*models.find((*item).second.model.parentUID)).second;
			(*item).second.parent = parent; //Add the parent to it's ID
			parent->children.push_back(&(*item).second);
		}
		else
		{ 
			std::string rootName;
			App->fileSystem->SplitFilePath(resource.assetsFile.c_str(), nullptr, &rootName);
			(*item).second.model.name = rootName.c_str();
			root = &(*item).second;
		}
	}
	Importer::SceneImporter::LoadSceneResource(*root); //Create the game objects using the tree structure, passing the root
}

void MResourceManager::LoadMesh(UID uid)
{
	std::string path = MESHES_PATH;
	path += std::to_string(uid) + ".mesh";

	char* buffer;
	App->fileSystem->Load(path.c_str(),&buffer);

	RMesh* mesh = new RMesh(uid);
	Importer::MeshImporter::Load(buffer,mesh);

	resources.emplace(uid, mesh);

	LOG("Mesh added into memory");
}

void MResourceManager::SaveResource(Resource* resource)
{
	GenerateMeatFile(resource); //idk where to do this
	AddResourceToLibrary(resource);
}

Resource* MResourceManager::RequestResource(uint uid)
{
	std::map<UID, Resource*>::iterator it = resources.find(uid);
	if (it != resources.end())
	{
		it->second->resourceData.referenceCount++;
		LOG("Resource %s has been referenced %d times", it->second->resourceData.assetsFile.c_str(), it->second->resourceData.referenceCount);
		return it->second;
	}
	return nullptr;
}

void MResourceManager::ReleaseResource(uint uid)
{

}

ResourceData MResourceManager::RequestLibraryResource(uint uid)
{
	std::map<UID, ResourceData>::iterator it = resourcesInLibrary.find(uid);
	if (it != resourcesInLibrary.end())
	{
		return it->second;
	}
	return ResourceData(); //Default has UID = 0
}

void MResourceManager::AddResourceToLibrary(Resource* resource)
{
	ResourceData res;
	res = resource->resourceData;
	resourcesInLibrary.emplace(res.UID, res);
}

void MResourceManager::AddResourceToLibraryFromMeta(const char* file)
{
	//std::string fullName = file;
	//fullName += ".meta";

	//Read meta and stor into new ResourceData and sore it
	char* buffer;
	if (App->fileSystem->Load(file, &buffer) == 0)
		return;

	JsonNode node(buffer);
	ResourceData resource;
	resource.UID = node.GetNumber("UID");

	if (resourcesInLibrary.find(resource.UID) != resourcesInLibrary.end() )
	{
		LOG("Tries to import file already in library: %s",file);
		return;
	}

	resource.type = (ResourceType)node.GetNumber("Type");
	resource.assetsFile = node.GetString("Assets File");
	resource.libraryFile = node.GetString("Library File");

	resourcesInLibrary.emplace(resource.UID, resource);
}

void MResourceManager::ImportAllFoundAssets(const char* basePath)
{
	std::vector<std::string> files,dirs; // read all assets folders and look inside

	App->fileSystem->DiscoverFiles(basePath, files, dirs);

	for (std::vector<std::string>::iterator i = files.begin(); i != files.end(); i++)
	{
		std::string tmp1 = basePath;
		tmp1 += "/" + (*i);
		ImportFile( tmp1.c_str() , (ResourceType)GetResourceTypeFromPath(tmp1.c_str()) );
	}

	for (std::vector<std::string>::iterator item = dirs.begin(); item != dirs.end();item++)
	{
		std::string tmp = basePath;
		tmp += "/" + (*item);
		ImportAllFoundAssets(tmp.c_str());
	}
}

uint MResourceManager::GetResourceTypeFromPath(const char* path)
{
	ResourceType ret = ResourceType::none;

	std::string extension;

	App->fileSystem->SplitFilePath(path, nullptr, nullptr, &extension);

	if (strstr(extension.c_str(), "fbx") != nullptr || strstr(extension.c_str(), "FBX") != nullptr) //Only importing .fbx files for now
	{
		ret = ResourceType::scene;
	}

	return (uint)ret;
}
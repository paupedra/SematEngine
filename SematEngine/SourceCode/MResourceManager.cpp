#include "Application.h"
#include "Globals.h"
#include "Resource.h"
#include "GameObject.h"
#include "Random.h"
#include "Config.h"
#include "Color.h"

#include "MFileSystem.h"
#include "MScene.h"

#include "CMaterial.h"

#include "RMaterial.h"
#include "RMesh.h"
#include "RTexture.h"
#include "RScene.h"
#include "RModel.h"
#include "RAnimation.h"

#include "IScene.h"
#include "ITexture.h"
#include "IMesh.h"

#include "MResourceManager.h"

//#include "Dependecies/mmgr/mmgr.h"

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
	//LoadResourceFromMeta("Assets/Mesh/Street environment_V01.FBX.meta");
	//LoadResourceFromMeta("Assets/Mesh/aniTest.fbx.meta");
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
		//Importer::SceneImporter::Import(str2.c_str());
	}

	if (strstr(str2.c_str(), ".png") != nullptr || strstr(".PNG", str2.c_str()) != nullptr)
	{
		//App->scene->selectedObject->GetComponent<CMaterial>()->GetMaterial()->SetTexture(Importer::TextureImp::Import(str2.c_str()));
	}
}

uint MResourceManager::ImportFile(const char* newFileInAssets, ResourceType type)
{
	int ret = 0;

	LOG("Started importing %s", newFileInAssets);

	if (type == ResourceType::none)
	{
		return ret;
	}

	if (FindMeta(newFileInAssets)) //When requesting to import a file we first check if it already has .meta therefore is already imported
	{
		std::string str = newFileInAssets;
		str += ".meta";
		ret = AddResourceToLibraryFromMeta(str.c_str());		//load file in this meta and add it to resourcesInLibrary
		return ret;
	}

	char* fileBuffer;
	uint size = App->fileSystem->Load(newFileInAssets,&fileBuffer);
	Resource* resource = CreateNewResource(newFileInAssets, type);

	switch (type)
	{
		case ResourceType::texture: Importer::TextureImp::Import(fileBuffer, (RTexture*)resource, size); break; //fills out RTexture and save
		case ResourceType::model: Importer::SceneImporter::ImportSceneResource(fileBuffer,(RScene*)resource,size); break; //Request all necessary files in scene
		case ResourceType::none: return ret; break;
	}
	LOG("Finished importing %s first", newFileInAssets);
	SaveResource(resource); 
	ret = resource->resourceData.UID;

	RELEASE_ARRAY(fileBuffer);
	RELEASE(resource);

	LOG("Finished importing %s", newFileInAssets);

	return ret;
}

uint MResourceManager::ImportModelResource(const char* newFileInAssets, ResourceType type)
{
	return 0;
}

Resource* MResourceManager::CreateNewResource(const char* assetsFile, ResourceType type)
{
	Resource* ret = nullptr;
	UID uid = Random::GenerateUID();

	switch (type) 
	{
		case ResourceType::texture:  ret = (Resource*) new RTexture(uid); break;
		case ResourceType::mesh:	 ret = (Resource*) new RMesh(uid); break;
		case ResourceType::material: ret = (Resource*) new RMaterial(uid); break;
		case ResourceType::model:	 ret = (Resource*) new RScene(uid); break;
	}
	if (ret != nullptr)
	{
		ret->resourceData.assetsFile = assetsFile;
		ret->resourceData.UID = uid;
		ret->resourceData.libraryFile = GenerateLibraryFile(ret);
	}

	return ret;
}

uint MResourceManager::ImportMaterial(const char* file,uint textureUID, Color color) //create material resource and set it up
{
	RMaterial* resource = (RMaterial*)CreateNewResource(file, ResourceType::material);
	uint uid = resource->resourceData.UID;

	//add color 
	resource->SetColor(color);
	
	resource->GenerateCustomFile(textureUID); //cff with the color and texture id
	

	//SaveResource(resource,false);

	return uid;
}

const char* MResourceManager::GenerateMetaFile(Resource* resource)
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
			Importer::TextureImp::Save((RTexture*)resource, resource->resourceData.libraryFile.c_str());
			break;
		case ResourceType::model:
			LOG("Generationg Scene meta file: %s", path.c_str());
			break;
		case ResourceType::material:
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
			path += TEXTURE_EXTENTION;
			break;

		case ResourceType::model:
			path = MODELS_PATH;
			path += std::to_string(resource->GetUID());
			path += MODEL_EXTENTION;
			break;
		case ResourceType::material:
			path = MATERIALS_PATH;
			path += std::to_string(resource->GetUID());
			path += MATERIAL_EXTENTION;
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
		case ResourceType::model: 
			LoadScene(resource);
			break;
		case ResourceType::mesh:
			LoadMesh(uid);
			break;
		case ResourceType::texture:
			LoadTexture(resource);
			return uid;
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
		case ResourceType::material:
			LoadMaterial(uid);
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

	RELEASE_ARRAY(buffer);
}

void MResourceManager::LoadMesh(UID uid)
{
	std::string path = MESHES_PATH;
	path += std::to_string(uid) + MESH_EXTENTION;

	char* buffer;
	App->fileSystem->Load(path.c_str(),&buffer);

	RMesh* mesh = new RMesh(uid);
	Importer::MeshImporter::Load(buffer,mesh);

	resources.emplace(uid, mesh);

	RELEASE_ARRAY(buffer);

	LOG("Mesh added into memory");
}

RMaterial* MResourceManager::LoadMaterial(UID uid) //this will be called when loading scene (resourceData.materialUID)
{
	std::string path = MATERIALS_PATH;
	path += std::to_string(uid) + MATERIAL_EXTENTION;

	char* buffer = nullptr;
	App->fileSystem->Load(path.c_str(), &buffer);

	//we have .material into buffer, now load texture and color into resource and add it to memory
	RMaterial* material = new RMaterial(uid);

	//read json
	JsonNode node(buffer);

	Color color;	//Color
	JsonArray colorArray = node.GetArray("Color");

	color.r = colorArray.GetNumber(0, 1);
	color.g = colorArray.GetNumber(1, 1);
	color.b = colorArray.GetNumber(2, 1);
	color.a = colorArray.GetNumber(3, 1);

	material->SetColor(color);

	uint textureUID = node.GetNumber("Texture UID");	//Texture

	ResourceData textureData = RequestLibraryResource(textureUID); //texture resource data

	//load the texture in memory --------------------
	char* buffer2 = nullptr;

	uint size = App->fileSystem->Load(textureData.libraryFile.c_str(), &buffer2); //.texture file in buffer
	
	RTexture* texture = new RTexture(textureData.UID);

	Importer::TextureImp::Load(textureData.libraryFile.c_str(),texture);

	resources.emplace(textureData.UID, texture);
	//------------------------------------------------

	material->SetTexture((RTexture*)RequestResource(textureData.UID));
	
	RELEASE_ARRAY(buffer);
	RELEASE_ARRAY(buffer2);

	return material;
}

void MResourceManager::LoadTexture(ResourceData resource)
{
	std::string path = TEXTURES_PATH;
	path += std::to_string(resource.UID) + TEXTURE_EXTENTION;

	char* buffer;
	uint size = App->fileSystem->Load(path.c_str(), &buffer);

	RTexture* texture = new RTexture(resource.UID);
	Importer::TextureImp::Import(buffer,texture,size);
	texture->usesTexture = true;
	resources.emplace(resource.UID, texture);

	LOG("Mesh added into memory");
}

void MResourceManager::SaveResource(Resource* resource, bool meta)
{
	LOG("Saving resource: %s", resource->resourceData.assetsFile.c_str());
	if(meta)
		GenerateMetaFile(resource); //idk where to do this

	AddResourceToLibrary(resource);
	LOG("Finished Saving resource: %s", resource->resourceData.assetsFile.c_str());
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

void MResourceManager::ReleaseResource(uint uid) //releases a resource from memory
{
	Resource* res = (*resources.find(uid)).second;
	res->CleanUp();

	resources.erase(uid);

	RELEASE(res);
}

uint MResourceManager::GetResourceUID(const char* metaFile)
{
	for (std::map<UID, ResourceData>::iterator it = resourcesInLibrary.begin(); it != resourcesInLibrary.end(); it++)
		if( strcmp((*it).second.assetsFile.c_str(),metaFile ) == 0)
			return (*it).second.UID;

	return 0;
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

void MResourceManager::DereferenceResource(uint uid)
{
	std::map<UID, Resource*>::iterator it = resources.find(uid);
	if (it != resources.end())
	{
		it->second->resourceData.referenceCount--;
		if (it->second->resourceData.referenceCount <= 0)
		{
			//release resource
			ReleaseResource(uid);
		}
	}
}

void MResourceManager::AddResourceToLibrary(Resource* resource)
{
	ResourceData res;
	res = resource->resourceData;
	resourcesInLibrary.emplace(res.UID, res);
}

int MResourceManager::AddResourceToLibraryFromMeta(const char* file)
{
	//std::string fullName = file;
	//fullName += ".meta";

	//Read meta and stor into new ResourceData and sore it
	char* buffer;
	if (App->fileSystem->Load(file, &buffer) == 0)
		return 0;

	JsonNode node(buffer);
	ResourceData resource;
	resource.UID = node.GetNumber("UID");
	uint uid = resource.UID;
	LOG("uid is %d", uid);

	if (resourcesInLibrary.find(resource.UID) != resourcesInLibrary.end() )
	{
		LOG("Tries to import file already in library: %s",file);
		return uid;
	}

	resource.type = (ResourceType)node.GetNumber("Type");
	resource.assetsFile = node.GetString("Assets File");
	resource.libraryFile = node.GetString("Library File");

	resourcesInLibrary.emplace(resource.UID, resource);
	return uid;
}

void MResourceManager::ImportAllFoundAssets(const char* basePath)
{
	std::vector<std::string> files,dirs; // read all assets folders and look inside

	App->fileSystem->DiscoverFiles(basePath, files, dirs);

	for (std::vector<std::string>::iterator i = files.begin(); i != files.end(); i++)
	{
		if (strstr((*i).c_str(),".meta" ) == nullptr) //only fo this with files that are not metas
		{
			std::string tmp1 = basePath;
			tmp1 += "/" + (*i);
			int ret = ImportFile(tmp1.c_str(), (ResourceType)GetResourceTypeFromPath(tmp1.c_str()));
			LOG("Hello I'm here %d",ret);
		}
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
		ret = ResourceType::model;
	}

	if (strstr(extension.c_str(), "png") != nullptr || strstr(extension.c_str(), "PNG") != nullptr || strstr(extension.c_str(), "tga") != nullptr || strstr(extension.c_str(), "TGA") != nullptr)
	{
		ret = ResourceType::texture;
	}

	return (uint)ret;
}
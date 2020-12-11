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

	uint u = Find(newFileInAssets);
	if (u != 0)
	{
		ret = u;
	}

	//Create resource
	
	char* fileBuffer;
	uint size = App->fileSystem->Load(newFileInAssets,&fileBuffer);
	Resource* resource = CreateNewResource(newFileInAssets, type);
	switch (type)
	{
		//case ResourceType::mesh: Importer::MeshImporter::Import();
		case ResourceType::texture: Importer::TextureImp::Import(fileBuffer,(RTexture*)resource,size);
	}
	
	resources.emplace(resource->GetUID(), resource);
	ret = resource->UID;
	
	RELEASE_ARRAY(fileBuffer);
	//save it
	
	return ret;
}

uint MResourceManager::GenerateNewUID()
{
	return 0;
}

Resource* MResourceManager::CreateNewResource(const char* assetsFile, ResourceType type)
{
	Resource* ret = nullptr;
	UID uid = Random::GenerateUID();

	switch (type) {
		case ResourceType::texture: ret = (Resource*) new RTexture(uid); break;
		case ResourceType::mesh: ret = (Resource*) new RMesh(uid); break;
		case ResourceType::material: ret = (Resource*) new RMaterial(uid); break;
	}
	if (ret != nullptr)
	{
		resources.insert(std::pair<UID,Resource*>(uid,ret)); //Assign resource to map
		ret->assetsFile = assetsFile;
		ret->UID = uid;
		GenerateMeatFile(ret,type); //idk where to do this
		ret->libraryFile = GenerateLibraryFile(ret,type); //generate meta file
	}

	return ret;
}

const char* MResourceManager::GenerateMeatFile(Resource* resource,ResourceType type)
{
	std::string ret;
	JsonNode node;

	switch (type)
	{
		case ResourceType::texture: 
				GenerateTextureMetaFile(resource,node);
			break;
	}

	return ret.c_str();
}

const char* MResourceManager::GenerateLibraryFile(Resource* resource, ResourceType type)
{
	std::string path = "";
	switch (type)
	{
		case ResourceType::texture: 
			path = TEXTURES_PATH;
			path += std::to_string(resource->GetUID());
			path += ".tex";
			Importer::TextureImp::Save((RTexture*)resource,path.c_str());
			break;
	}
	
	return path.c_str();
}

void MResourceManager::GenerateTextureMetaFile(Resource* resource,JsonNode node)
{

	node.AddNumber("UID", resource->UID);
	std::string file;
	std::string extension;
	App->fileSystem->SplitFilePath(resource->assetsFile.c_str(), nullptr, &file,&extension);


	std::string finalFile = "Assets/Textures/";
	finalFile += file + "." + extension + ".meta";

	LOG("Generationg Texture meta file: %s",finalFile.c_str());

	char* buffer;
	uint size = node.Serialize(&buffer);
	App->fileSystem->Save(finalFile.c_str(),buffer,size);
}

uint MResourceManager::Find(const char* fileInAssets)
{
	uint ret = 0;

	//look for the file's meta file and extract the uid if it's found
	std::string file = fileInAssets;

	file += ".meta";
	char* buffer;
	if (App->fileSystem->Load(file.c_str(), &buffer) == 0) //0 if failed
	{
		
	}
	else
	{
		JsonNode node(buffer);
		ret = node.GetNumber("UID");

		if (RequestResource(ret) == nullptr) //If the uid is not in resources then this meta is old
		{
			ret = 0; //return 0 so that we load it
			return ret;
		}
	}

	return ret;
}

void MResourceManager::ImportScene()
{

}

//const Resource* MResourceManager::RequestResource(uint uid) const
//{
//	const Resource* ret = nullptr;
//
//	ret = resources.find(uid)->second;
//
//	return ret;
//}

Resource* MResourceManager::RequestResource(uint uid)
{
	std::map<UID, Resource*>::iterator it = resources.find(uid);
	if (it != resources.end())
	{
		LOG("Resource has %d references", it->second->referenceCount);
		it->second->referenceCount++;
		LOG("Resource referenced %d times", it->second->referenceCount);
		return it->second;
	}
	return nullptr;
}

void MResourceManager::ReleaseResource(uint uid)
{

}

void GenerateMeta()
{

}
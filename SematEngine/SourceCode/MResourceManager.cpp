#include "Application.h"
#include "Globals.h"
#include "Resource.h"
#include "GameObject.h"
#include "Random.h"

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
	//Create resource
	UID ret = 0;
	char* fileBuffer;
	uint size = App->fileSystem->Load(newFileInAssets,&fileBuffer);
	Resource* resource = CreateNewResource(newFileInAssets, type);
	switch (type)
	{
		//case ResourceType::mesh: Importer::MeshImporter::Import();
		case ResourceType::texture: Importer::TextureImp::Import(fileBuffer,(RTexture*)resource,size);
	}
	

	ret = resource->UID;
	//delete[] fileBuffer;
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
		ret->libraryFile = GenerateMeatFile(ret); //generate meta file
	}

	return ret;

}

const char* MResourceManager::GenerateMeatFile(Resource* resource)
{
	std::string ret;

	switch (resource->GetType())
	{
		//case ResourceType::
	}

	return ret.c_str();
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
		it->second->referenceCount++;
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
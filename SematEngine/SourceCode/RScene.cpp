#include "Application.h"
#include "Resource.h"
#include "Config.h"

#include "MFileSystem.h"
#include "RModel.h"
#include "RScene.h"

RScene::RScene()
{
	resourceData.type = ResourceType::model;
	
}

RScene::RScene(uint UID) : Resource(UID)
{
	resourceData.type = ResourceType::model;
}

RScene::~RScene()
{

}

void RScene::CleanUp()
{

}

UID RScene::GenerateCustomFile()
{
	JsonNode node;

	node.AddNumber("Animation Collection",animationsCollection.GetUID());

	JsonArray meshesJson = node.InitArray("Meshes");

	for (auto it = meshes.begin(); it != meshes.end(); it++)
	{
		meshesJson.AddNumber(*it);
	}

	JsonArray materialsJson = node.InitArray("Materials");

	for (auto it = materials.begin(); it != materials.end(); it++)
	{
		materialsJson.AddNumber(*it);
	}

	JsonArray modelsJson = node.InitArray("Models");

	for(std::vector<RModel>::iterator model = models.begin() ;model != models.end();model++)
	{ 
		JsonNode modelNode = modelsJson.AddNode();

		modelNode.AddNumber("UID", (*model).uid);
		modelNode.AddNumber("Parent UID", (*model).parentUID);
		modelNode.AddString("Name", (*model).name.c_str());
		//transform

		JsonArray transformArray = modelNode.InitArray("Transform");
		for (int i = 0; i< 16;i++)
		{
			transformArray.AddNumber((*model).transform.ptr()[i]);
		}

		if((*model).meshUID != -1)
			modelNode.AddNumber("Mesh ID", meshes[(*model).meshUID]); //Loop to support various meshes
		else
			modelNode.AddNumber("Mesh ID", 0);

		modelNode.AddNumber("Material ID", materials[(*model).materialUID]);
	}

	char* buffer;
	uint size = node.Serialize(&buffer);

	std::string fileName = MODELS_PATH;
	fileName += std::to_string(resourceData.UID);
	fileName += MODEL_EXTENTION;

	App->fileSystem->Save(fileName.c_str(),buffer,size);
	
	return resourceData.UID;
}
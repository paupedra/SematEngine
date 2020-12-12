#include "Application.h"
#include "Resource.h"
#include "Config.h"

#include "MFileSystem.h"

#include "RScene.h"

RScene::RScene()
{
	resourceData.type = ResourceType::scene;
}

RScene::RScene(uint UID) : Resource(UID)
{
	resourceData.type = ResourceType::scene;
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

	JsonArray modelsJson = node.InitArray("Models");

	for(std::vector<RModel>::iterator model = models.begin() ;model != models.end();model++)
	{ 
		JsonNode modelNode = modelsJson.AddNode();

		modelNode.AddNumber("UID", (*model).uid);
		modelNode.AddNumber("Parent UID", (*model).parentUID);
		modelNode.AddString("Name", (*model).name.c_str());
		//transform

		JsonArray transformArray = modelNode.InitArray("Transform");
		for (int i = 0; i < 4; i++)
		{
			for (int y = 0; y < 4; y++)
			{
				transformArray.AddNumber((*model).transform[i][y]);
			}
		}

		modelNode.AddNumber("Mesh ID", meshes[(*model).mesheUID]); //Loop to support various meshes
		modelNode.AddNumber("Material ID", (*model).materialUID);
	}

	char* buffer;
	uint size = node.Serialize(&buffer);

	std::string fileName = SCENES_PATH;
	fileName += std::to_string(resourceData.UID);
	fileName += ".scene";

	App->fileSystem->Save(fileName.c_str(),buffer,size);
	
	return resourceData.UID;
}
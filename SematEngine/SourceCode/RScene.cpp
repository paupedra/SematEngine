#include "Resource.h"

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
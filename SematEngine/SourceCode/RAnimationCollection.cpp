#include "Application.h"
#include "Random.h"
#include "Globals.h"
#include "Resource.h"

#include "MFileSystem.h"

#include "Config.h"

#include "RAnimationCollection.h"


RAnimationCollection::RAnimationCollection()
{
	resourceData.UID = Random::GenerateUID();
}

RAnimationCollection::RAnimationCollection(uint UID) : Resource(UID,ResourceType::animationCollection)
{
	
}

RAnimationCollection::~RAnimationCollection()
{

}

void RAnimationCollection::AddAnimation(UID UID)
{
	animations.push_back(UID);
}

void RAnimationCollection::CleanUp()
{

}

UID RAnimationCollection::GenerateCustomFile()
{
	//array of animations uid 
	JsonNode node;

	JsonArray animationsJson = node.InitArray("Animations");

	for (std::vector<UID>::iterator it = animations.begin(); it != animations.end(); it++)
	{
		animationsJson.AddNumber((*it));
	}

	std::string path = ANIMATIONS_PATH;
	path += std::to_string(resourceData.UID);
	path += ANIMATION_EXTENSION;

	char* buffer = nullptr;
	uint size = node.Serialize(&buffer);

	App->fileSystem->Save(path.c_str(), buffer, size);

	RELEASE_ARRAY(buffer);

	return resourceData.UID;
}
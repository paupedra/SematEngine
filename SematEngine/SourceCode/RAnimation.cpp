#include "Resource.h"
#include "Globals.h"

#include "RAnimation.h"

#include "IAnimation.h"


RAnimation::RAnimation()
{
	resourceData.type = ResourceType::animation;
}

RAnimation::RAnimation(uint UID) : Resource(UID)
{
	resourceData.type = ResourceType::animation;
}

RAnimation::~RAnimation()
{

}

void RAnimation::CleanUp()
{

}

UID RAnimation::GenerateCustomFile()
{
	std::string fileName = ANIMATIONS_PATH;
	fileName += std::to_string(resourceData.UID);
	fileName += ANIMATION_EXTENSION;
	Importer::AnimationImporter::Save(this, fileName.c_str());

	return resourceData.UID;
}
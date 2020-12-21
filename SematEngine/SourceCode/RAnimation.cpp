#include "Resource.h"
#include "Globals.h"
#include "RAnimation.h"


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

UID RAnimation::GenerateCustomFile(UID textureUID)
{
	return 0;
}
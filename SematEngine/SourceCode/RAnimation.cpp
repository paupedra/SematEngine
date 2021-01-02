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

void AnimationClip::SetStartKey(float _startKey)
{
	if (_startKey < 0)
		_startKey = 0;

	if (_startKey >= owner->duration)
		_startKey = owner->duration - 1;

	if (_startKey > endKey)
		endKey = _startKey + 1;

	startKey = _startKey;
}

void AnimationClip::SetEndKey(float _endKey)
{
	if (_endKey < startKey)
		_endKey = startKey + 1;

	if (_endKey > owner->duration)
		_endKey = owner->duration;

	endKey = _endKey;
}

void AnimationClip::SetSpeed(float _speed)
{
	speed = _speed;
}
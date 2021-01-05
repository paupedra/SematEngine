#include "Application.h"
#include "Globals.h"
#include "Config.h"
#include "Component.h"
#include "GameObject.h"
#include "Resource.h"

#include "MRenderer3D.h"

#include "RAnimationCollection.h"
#include "RAnimation.h"

#include "CAnimator.h"
#include "CTransform.h"

CAnimator::CAnimator(GameObject* owner) : Component(ComponentType::ANIMATOR, owner)
{

}

CAnimator::~CAnimator()
{

}

void CAnimator::Update(float dt)
{
	if (currentAnimation == nullptr)
		return;

	float thisDt = 0;

	if (App->IsInPlayMode())
		thisDt = dt;
	else
		thisDt = App->GetDt();

	if (!transitionQueue.empty() && !transitioning)
	{
		transitioning = true;
		currentTransition = &transitionQueue.front();
	}

	if (transitioning && !paused)
	{
		UpdateTransitionTime(thisDt);

		//check if transition time is bigger than transition duration
		if (transitionTime > currentTransition->duration)
		{
			SetCurrentClip(currentTransition->clip);
			currentClipTicks = transitionClipTicks;
			currentClipTime = transitionClipTime;

			transitionTime = 0;

			currentTransition = nullptr;
			transitioning = false;
			transitionQueue.pop();
		}
		else
		{
			UpdateTransitionBones();
		}
	}

	//update current animation time/ticks
	if (!paused && !transitioning)
	{
		currentClipTime += thisDt * (double)playbackSpeed;

		durationInSeconds = (currentClip->endKey - currentClip->startKey) / currentClip->speed;

		uint timeMs = currentClipTime * 1000;
		uint durationMs = (durationInSeconds * 1000);

		if (timeMs > durationMs) //ticks wrap arround the duration
		{
			//should take into account amount of times ticks is bigger then duration ( % ?) but they are not int
			currentClipTime = timeMs % durationMs;
			currentClipTime /= 1000;
		}

		currentClipTicks = currentClipTime  * currentClip->speed + currentClip->startKey; //augmented ticks

		//update bones positions
		UpdateBones();
	}

	//move and draw the bones according to current animation
	if (drawBones)
	{
		DrawBones();
	}
}

void CAnimator::CleanUp()
{

}

void CAnimator::OnPlay()
{
	if (currentAnimation == nullptr)
		if(!animations.empty())
			SetCurrentAnimation(animations.front());

	Play();
}

void CAnimator::OnStop()
{
	Stop();
}

void CAnimator::Serialize(JsonNode* node)
{
	node->AddNumber("Collection UID", collectionUID);
}

void CAnimator::AddAnimation(RAnimation* newAnimation)
{
	animations.push_back(newAnimation);
}

void CAnimator::AddClip(float startKey, float endKey, float speed)
{
	AnimationClip newClip(currentAnimation,startKey, endKey, speed);

	newClip.SetStartKey(startKey);
	newClip.SetEndKey(endKey);
	newClip.SetSpeed(speed);

	currentAnimation->clips.push_back(newClip);
}

void CAnimator::AddTransition(AnimationClip* clip, float duration)
{
	AnimationTransition newTransition(clip, duration);
	transitionQueue.push(newTransition);
}

void CAnimator::LinkBones()
{
	linkedBones.clear();
	rootBone = nullptr;

	//find bones one by one and FindChild
	for (std::map<std::string,Bone>::const_iterator it = currentAnimation->bones.begin(); it != currentAnimation->bones.end(); it++)
	{
		GameObject* recipient = nullptr;
		owner->GetChildByName(it->first.c_str(),&recipient);

		linkedBones.emplace(it->first,recipient);
	}

	for (int i = 0; i < owner->children.size(); i++)
	{
		std::map<std::string, GameObject*>::iterator b = linkedBones.find(owner->children[i]->GetName());
		if ( b != linkedBones.end())
		{
			rootBone = b->second;
		}
	}
}

void CAnimator::DrawBones()
{
	for (std::map<std::string,GameObject*>::const_iterator it = linkedBones.begin(); it != linkedBones.end(); it++)
	{
		if (it->second != nullptr)
		{
			for (int i = 0; i < it->second->children.size(); i++)
			{
				App->renderer3D->DrawLine(it->second->transform->GetGlobalPosition(), it->second->children[i]->transform->GetGlobalPosition());
			}
		}
	}
}

void CAnimator::UpdateBones()
{
	std::map<std::string, Bone>::const_iterator bone = currentAnimation->bones.begin();
	for (bone; bone != currentAnimation->bones.end(); bone++)
	{
		std::map<std::string, GameObject*>::const_iterator it = linkedBones.find(bone->first);
		if (it == linkedBones.end())
			break;
		else
			if (it->second == nullptr)
				break;

		float3 position = UpdateBonePosition(bone, it);

		float3 scale = UpdateBoneScale(bone,it);

		Quat rotation = UpdateBoneRotation(bone, it);

		float4x4 mat = float4x4::FromTRS(position, rotation, scale);
		it->second->transform->SetLocalTransform(mat);
	}
}

float3 CAnimator::UpdateBonePosition(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it)
{
	float3 finalPosition = ComputeBonePositionInterpolation(bone,it,currentClipTicks,currentClipTime);
	return finalPosition;
}

float3 CAnimator::UpdateBoneScale(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it)
{
	float3 prevScale, nextScale, finalScale;
	prevScale = nextScale = finalScale = float3::zero;

	double prevTicks, nextTicks;
	prevTicks = nextTicks = 0;

	std::map<double, float3>::const_iterator scale = bone->second.scaleKeys.lower_bound(currentClipTicks);
	scale--;

	if (scale != bone->second.scaleKeys.end())
	{
		prevScale = scale->second;
		prevTicks = scale->first;
		scale++;
	}

	if (scale != bone->second.scaleKeys.end()) //it there is an upper (not last)
	{
		nextScale = scale->second;
		nextTicks = scale->first;

		float a = (double)currentClipTime - TicksToTime(prevTicks);
		float b = TicksToTime(nextTicks) - TicksToTime(prevTicks);
		float lerpT = a / b;

		if (lerpT > 1)
			lerpT = 1;
		if (lerpT < 0)
			lerpT = 0;

		finalScale = float3::Lerp(prevScale, nextScale, lerpT); //t is between 0 and 1 (0 is previous key time 1 is next key time)
	}
	else //blending here
	{
		finalScale = prevScale;
	}

	return finalScale;
}

Quat CAnimator::UpdateBoneRotation(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it)
{
	Quat finalRotation = ComputeBoneRotationInterpolation(bone, it, currentClipTicks, currentClipTime);
	return finalRotation;
}

void CAnimator::UpdateTransitionTime(float dt)
{
	transitionTime += dt;

	//current animation timers ----------------------------------------------------------------------
	currentClipTime += dt * (double)playbackSpeed;
	durationInSeconds = (currentClip->endKey - currentClip->startKey) / currentClip->speed;
	uint timeMs = currentClipTime * 1000;
	uint durationMs = (durationInSeconds * 1000);

	if (timeMs > durationMs) //ticks wrap arround the duration
	{
		currentClipTime = timeMs % durationMs;
		currentClipTime /= 1000;
	}
	currentClipTicks = currentClipTime * currentClip->speed + currentClip->startKey; //augmented ticks

	//Transition timers -----------------------------------------------------------------------------
	transitionClipTime += dt * (double)playbackSpeed;
	transitionDurationInSeconds = (currentTransition->clip->endKey - currentTransition->clip->startKey) / currentTransition->clip->speed;
	timeMs = transitionClipTime * 1000;
	durationMs = (transitionDurationInSeconds * 1000);

	if (timeMs > durationMs) //ticks wrap arround the duration
	{
		transitionClipTime = timeMs % durationMs;
		transitionClipTime /= 1000;
	}
	transitionClipTicks = transitionClipTime * currentTransition->clip->speed + currentTransition->clip->startKey; //augmented ticks
}

void CAnimator::UpdateTransitionBones()
{
	std::map<std::string, Bone>::const_iterator bone = currentAnimation->bones.begin();
	for (bone; bone != currentAnimation->bones.end(); bone++)
	{
		std::map<std::string, GameObject*>::const_iterator it = linkedBones.find(bone->first);
		if (it == linkedBones.end())
			break;
		else
			if (it->second == nullptr)
				break;

		UpdateTransitionBonePosition(bone, it);

		UpdateTransitionBoneRotation(bone, it);
	}
}

void CAnimator::UpdateTransitionBonePosition(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it)
{
	float3 finalPosition, finalCurrentPosition, finalTransitionPosition = float3::zero;

	finalCurrentPosition = ComputeBonePositionInterpolation(bone, it,currentClipTicks,currentClipTime);
	finalTransitionPosition = ComputeBonePositionInterpolation(bone, it, transitionClipTicks, transitionClipTime);

	float lerpT = transitionTime / currentTransition->duration;
	finalPosition = float3::Lerp(finalCurrentPosition, finalTransitionPosition, lerpT);

	it->second->transform->SetPosition(finalPosition);
}

void CAnimator::UpdateTransitionBoneScale(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it)
{
	float3 finalScale, finalCurrentScale, finalTransitionScale = float3::zero;

	finalCurrentScale = ComputeBoneScaleInterpolation(bone, it, currentClipTicks, currentClipTime);
	finalTransitionScale = ComputeBoneScaleInterpolation(bone, it, transitionClipTicks, transitionClipTime);

	float lerpT = transitionTime / currentTransition->duration;
	finalScale = float3::Lerp(finalCurrentScale, finalTransitionScale, lerpT);

	it->second->transform->SetScale(finalScale);
}

void CAnimator::UpdateTransitionBoneRotation(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it)
{
	Quat finalRotation = Quat::identity;
	Quat finalCurrentRotation = Quat::identity;

	finalCurrentRotation = ComputeBoneRotationInterpolation(bone, it, currentClipTicks, currentClipTime);

	Quat finalTransitionPosition = Quat::identity;

	finalTransitionPosition = ComputeBoneRotationInterpolation(bone, it, transitionClipTicks, transitionClipTime);

	float lerpT = transitionTime / currentTransition->duration;
	finalRotation = Quat::Slerp(finalCurrentRotation, finalTransitionPosition, lerpT);

	it->second->transform->SetRotation(finalRotation);
}

float3 CAnimator::ComputeBonePositionInterpolation(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it,double ticks,double time)
{
	float3 prevPosition, nextPosition, finalPosition ;
	prevPosition = nextPosition = finalPosition = float3::zero;

	double prevTicks, nextTicks;
	prevTicks = nextTicks = 0;

	std::map<double, float3>::const_iterator pos = bone->second.positionKeys.lower_bound(ticks);
	pos--;

	if (pos != bone->second.positionKeys.end())
	{
		prevPosition = pos->second;
		prevTicks = pos->first;
		pos++;
	}

	if (pos != bone->second.positionKeys.end()) //it there is an upper (not last)
	{
		nextPosition = pos->second;
		nextTicks = pos->first;

		float a = (double)time - TicksToTime(prevTicks);
		float b = TicksToTime(nextTicks) - TicksToTime(prevTicks);
		float lerpT = a / b;

		if (lerpT > 1)
			lerpT = 1;
		if (lerpT < 0)
			lerpT = 0;

		finalPosition = float3::Lerp(prevPosition, nextPosition, lerpT); //t is between 0 and 1 (0 is previous key time 1 is next key time)
	}
	else //blending here
	{
		finalPosition = prevPosition;
	}

	return finalPosition;
}

float3 CAnimator::ComputeBoneScaleInterpolation(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it, double ticks, double time)
{
	float3 prevScale,nextScale, finalScale ;
	prevScale = nextScale = finalScale = float3::zero;

	double prevTicks, nextTicks;
	prevTicks = nextTicks = 0;

	std::map<double, float3>::const_iterator sca = bone->second.scaleKeys.lower_bound(ticks);
	sca--;

	if (sca != bone->second.scaleKeys.end())
	{
		prevScale = sca->second;
		prevTicks = sca->first;
		sca++;
	}

	if (sca != bone->second.scaleKeys.end()) //it there is an upper (not last)
	{
		nextScale = sca->second;
		nextTicks = sca->first;

		float a = (double)time - TicksToTime(prevTicks);
		float b = TicksToTime(nextTicks) - TicksToTime(prevTicks);
		float lerpT = a / b;

		if (lerpT > 1)
			lerpT = 1;
		if (lerpT < 0)
			lerpT = 0;

		finalScale = float3::Lerp(prevScale, nextScale, lerpT); //t is between 0 and 1 (0 is previous key time 1 is next key time)
	}
	else //blending here
	{
		finalScale = prevScale;
	}

	return finalScale;
}

Quat CAnimator::ComputeBoneRotationInterpolation(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it, double ticks, double time)
{
	Quat prevRotation, nextRotation, finalRotation ;
	prevRotation = nextRotation = finalRotation = Quat::identity;

	double prevTicks, nextTicks;
	prevTicks = nextTicks = 0;

	std::map<double, Quat>::const_iterator rot = bone->second.quaternionKeys.lower_bound(ticks);
	rot--;

	if (rot != bone->second.quaternionKeys.end())
	{
		prevRotation = rot->second;
		prevTicks = rot->first;
		rot++;
	}

	if (rot != bone->second.quaternionKeys.end()) //it there is an upper (not last)
	{
		nextRotation = rot->second;
		nextTicks = rot->first;

		float a = (double)time - TicksToTime(prevTicks);
		float b = TicksToTime(nextTicks) - TicksToTime(prevTicks);
		float lerpT = a / b;

		if (lerpT > 1)
			lerpT = 1;
		if (lerpT < 0)
			lerpT = 0;

		finalRotation = Quat::Slerp(prevRotation, nextRotation, lerpT); //t is between 0 and 1 (0 is previous key time 1 is next key time)
	}
	else //blending here
	{
		finalRotation = prevRotation;
	}
	return finalRotation;
}

void CAnimator::Play()
{
	if (currentAnimation != nullptr)
	{
		paused = false;

		if (currentClip == nullptr)
		{
			if (!currentAnimation->clips.empty())
			{
				SetCurrentClip(&currentAnimation->clips.front());
			}
			else
			{
				paused = true;
			}
		}
	}
}

void CAnimator::Pause()
{
	if (currentAnimation != nullptr)
		paused = true;
}

void CAnimator::Stop()
{
	if (currentAnimation == nullptr)
		return;

	currentClipTime = 0;
	currentClipTicks = 0;
	paused = true;
}

double CAnimator::TicksToTime(double ticks) //converts animation ticks to equivalent time
{
	float ret = 0;

	ret = (ticks / currentClip->speed);

	return ret;
}

double CAnimator::TimeToTicks(double time) //converts animation time to equivalent ticks
{
	return 0;
}

void CAnimator::SetCurrentAnimation(RAnimation* animation)
{
	currentAnimation = animation;
	LinkBones();
	
	if (animation->clips.empty())
	{
		AddClip(0, currentAnimation->duration, currentAnimation->speed);
	}

	SetCurrentClip(animation->clips.begin()._Ptr);
}

void CAnimator::SetCurrentClip(AnimationClip* clip)
{
	currentClip = clip;
	
}

void CAnimator::SetPlaybackSpeed(float speed)
{
	playbackSpeed = speed;
}

double CAnimator::GetAnimationSpeed() const 
{
	if (currentAnimation != nullptr)
		return currentAnimation->speed;
	return 0;
}

double CAnimator::GetAnimationDuration()const
{
	if (currentAnimation != nullptr)
		return currentAnimation->duration;
	return 0;
}

const char* CAnimator::GetAnimationName()const
{
	if (currentAnimation != nullptr)
		return currentAnimation->name.c_str();
	return "No animation selected";
}

std::vector<AnimationClip>* CAnimator::GetClips()
{ 
	return &currentAnimation->clips;
}
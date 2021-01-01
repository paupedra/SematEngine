#include "Application.h"
#include "Globals.h"
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

	//update current animation time/ticks
	if (!paused)
	{
		currentAnimationTime += dt * (double)playbackSpeed;

		uint timeMs = currentAnimationTime * 1000;
		uint durationMs = (durationInSeconds * 1000);

		if (timeMs > durationMs) //ticks wrap arround the duration
		{
			//should take into account amount of times ticks is bigger then duration ( % ?) but they are not int
			currentAnimationTime = timeMs % durationMs;
			currentAnimationTime /= 1000;
		}

		currentAnimationTicks = currentAnimationTime  * currentAnimation->speed; //augmented ticks

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

void CAnimator::AddAnimation(RAnimation* newAnimation)
{
	animations.push_back(newAnimation);
}

void CAnimator::AddChop(float startKey, float endKey, float speed)
{
	AnimationChop newChop(currentAnimation,startKey, endKey, speed);

	newChop.SetStartKey(startKey);
	newChop.SetEndKey(endKey);
	newChop.SetSpeed(speed);

	chops.push_back(newChop);
}

void CAnimator::AddTransition(AnimationChop* chop, float duration)
{
	AnimationTransition newTransition(chop, duration);
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

		UpdateBonePosition(bone, it);

	

		UpdateBoneRotation(bone, it);
		
	}
}

void CAnimator::UpdateBonePosition(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it)
{
	float3 prevPosition = float3::zero;
	float3 nextPosition = float3::zero;
	float3 finalPosition = float3::zero;

	double prevTicks = 0;
	double nextTicks = 0;

	std::map<double, float3>::const_iterator pos = bone->second.positionKeys.lower_bound(currentAnimationTicks);
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

		float a = (double)currentAnimationTime - TicksToTime(prevTicks);
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

	it->second->transform->SetPosition(finalPosition);
}

void CAnimator::UpdateBoneScale(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it)
{
	float3 prevScale = float3::zero;
	float3 nextScale = float3::zero;
	float3 finalScale = float3::zero;

	double prevTicks = 0;
	double nextTicks = 0;

	std::map<double, float3>::const_iterator scale = bone->second.scaleKeys.lower_bound(currentAnimationTicks);
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

		float a = (double)currentAnimationTime - TicksToTime(prevTicks);
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

	it->second->transform->SetPosition(finalScale);
}

void CAnimator::UpdateBoneRotation(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it)
{
	//rotation
	Quat prevRotation = Quat::identity;
	Quat nextRotation = Quat::identity;
	Quat finalRotation = Quat::identity;

	double prevTicks = 0;
	double nextTicks = 0;

	std::map<double, Quat>::const_iterator rot = bone->second.quaternionKeys.lower_bound(currentAnimationTicks);
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

		float a = (double)currentAnimationTime - TicksToTime(prevTicks);
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

	it->second->transform->SetRotation(finalRotation);
}

void CAnimator::Play()
{
	if(currentAnimation != nullptr)
		paused = false;
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

	currentAnimationTime = 0;
	currentAnimationTicks = 0;
	paused = true;
}

double CAnimator::TicksToTime(double ticks) //converts animation ticks to equivalent time
{
	float ret = 0;

	ret = (ticks / currentAnimation->speed);

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
	durationInSeconds = currentAnimation->duration / currentAnimation->speed;
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

std::vector<AnimationChop> CAnimator::GetChops()
{ 
	return chops; 
}
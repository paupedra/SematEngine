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
		currentAnimationTime += dt/2;

		currentAnimationTicks += (dt/2) * currentAnimation->speed; //augmented ticks

		if (currentAnimationTicks > currentAnimation->duration) //ticks wrap arround the duration
		{
			//should take into account amount of times ticks is bigger then duration ( % ?) but they are not int
			currentAnimationTicks = (uint)currentAnimationTicks % (uint)currentAnimation->duration;
		}

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

void CAnimator::SetCurrentAnimation(RAnimation* animation)
{
	currentAnimation = animation;
	LinkBones(); 
	durationInSeconds = currentAnimation->duration / currentAnimation->speed ;
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
	uint aniamtionTimeMs = currentAnimationTime * 1000;


	float animationTime =  0; //time of current animation in seconds

	std::map<std::string, Bone>::const_iterator bone = currentAnimation->bones.begin();
	for (bone; bone != currentAnimation->bones.end(); bone++)
	{

		std::map<std::string, GameObject*>::const_iterator it = linkedBones.find(bone->first);
		if (it == linkedBones.end())
			break;
		else
			if (it->second == nullptr)
				break;

		UpdateBonePosition(bone, it,animationTime);

		//scale
		float3 newScale = float3::zero;

		std::map<double, float3>::const_iterator sca = bone->second.scaleKeys.lower_bound(currentAnimationTicks);

		if (sca != bone->second.scaleKeys.end())
		{
			newScale = sca->second;
			it->second->transform->SetScale(newScale);
		}

		UpdateBoneRotation(bone, it, animationTime);
		
	}
}

void CAnimator::UpdateBonePosition(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it, float time)
{
	//position
	float3 prevPosition = float3::zero;
	float3 nextPosition = float3::zero;
	float3 finalPosition = float3::zero;

	double prevTicks = 0;
	double nextTicks = 0;

	std::map<double, float3>::const_iterator i = bone->second.positionKeys.lower_bound(currentAnimationTicks); //get prev

	if (i != bone->second.positionKeys.end())
	{
		prevPosition = i->second;
		prevTicks = i->first;
	}
		
	i = bone->second.positionKeys.upper_bound(currentAnimationTicks);

	if (i != bone->second.positionKeys.end()) //it there is an upper (not last)
	{
		nextPosition = i->second;
		nextTicks = i->first;

		float lerpT = (TicksToTime(prevTicks) - (double)time) / (TicksToTime(prevTicks) - TicksToTime(nextTicks));

		if (lerpT > 1)
		{
			lerpT = 1;
		}

		finalPosition = float3::Lerp(prevPosition, nextPosition, lerpT); //t is between 0 and 1 (0 is previous key time 1 is next key time)
		//finalPosition = prevPosition;
	}
	else //blending here
	{
		finalPosition = prevPosition;
	}

	it->second->transform->SetPosition(finalPosition);
}

void CAnimator::UpdateBoneRotation(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it, float time)
{
	//rotation
	Quat prevRotation = Quat::identity;
	Quat nextRotation = Quat::identity;
	Quat finalRotation = Quat::identity;

	double prevTicks = 0;
	double nextTicks = 0;

	std::map<double, Quat>::const_iterator rot = bone->second.quaternionKeys.lower_bound(currentAnimationTicks);

	if (rot != bone->second.quaternionKeys.end())
	{
		prevRotation = rot->second;
		prevTicks = rot->first;
	}

	rot = bone->second.quaternionKeys.upper_bound(currentAnimationTicks);

	if (rot != bone->second.quaternionKeys.end()) //it there is an upper (not last)
	{
		nextRotation = rot->second;
		nextTicks = rot->first;

		float lerpT = (TicksToTime(prevTicks) - time) / (TicksToTime(prevTicks) - TicksToTime(nextTicks));

		if (lerpT > 1)
			lerpT = 1;

		finalRotation = Quat::Slerp(prevRotation, nextRotation, lerpT); //t is between 0 and 1 (0 is previous key time 1 is next key time)
		//finalPosition = prevPosition;
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

	ret = ticks / currentAnimation->speed;

	return ret;
}

double CAnimator::TimeToTicks(double time) //converts animation time to equivalent ticks
{
	return 0;
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
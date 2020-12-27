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
		currentAnimationTime += dt;

		currentAnimationTicks += dt * currentAnimation->speed; //augmented ticks

		if (currentAnimationTicks > currentAnimation->duration) //ticks wrap arround the duration
		{
			//should take into account amount of times ticks is bigger then duration ( % ?) but they are not int
			currentAnimationTicks = (uint)currentAnimationTicks % (uint)currentAnimation->duration;
		}
	}

	//move and draw the bones according to current animation
	if (drawBones)
	{
		DrawBones();
	}

	//update bones positions

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
		for (int i = 0; i < it->second->children.size(); i++)
		{
			App->renderer3D->DrawLine(it->second->transform->GetPosition(), it->second->children[i]->transform->GetPosition());
		}
	}
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
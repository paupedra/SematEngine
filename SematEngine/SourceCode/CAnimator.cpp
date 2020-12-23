#include "Component.h"
#include "GameObject.h"
#include "Resource.h"

#include "RAnimationCollection.h"

#include "CAnimator.h"


CAnimator::CAnimator(GameObject* owner) : Component(ComponentType::ANIMATOR, owner)
{

}

CAnimator::~CAnimator()
{

}

void CAnimator::Update()
{

}

void CAnimator::CleanUp()
{

}
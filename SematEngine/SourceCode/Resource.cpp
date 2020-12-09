#include "Resource.h"


Resource::Resource()
{

}

Resource::Resource(uint UID) : UID(UID)
{

}

Resource::~Resource()
{

}

void Resource::SetUID(uint UID)
{
	this->UID = UID;
}

UID Resource::GetUID()const
{
	return UID;
}

ResourceType Resource::GetType()const
{
	return type;
}
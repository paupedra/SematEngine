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

uint Resource::GetUID()const
{
	return UID;
}
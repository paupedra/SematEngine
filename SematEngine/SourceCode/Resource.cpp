#include "Resource.h"


Resource::Resource()
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
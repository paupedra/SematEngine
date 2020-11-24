#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "Globals.h"

enum class ResourceType
{
	material,
	texture,
	none
};

class Resource
{
public:

	Resource();
	~Resource();

	void SetUID(uint UID);

	uint GetUID()const;

private:

	uint UID = 0;
	ResourceType type = ResourceType::none;
};
#endif //__RESOURCE_H__

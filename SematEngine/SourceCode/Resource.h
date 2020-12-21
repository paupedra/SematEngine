#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "Globals.h"
#include <string>
class JsonNode;

enum class ResourceType
{
	material,
	texture,
	mesh,
	model,
	scene,
	animation,
	none
};

struct ResourceData
{
	UID UID = 0;
	ResourceType type = ResourceType::none;
	std::string assetsFile;	 //File loaded from
	std::string libraryFile; //uint string
	uint referenceCount = 0; //Amount of times this resource is referenced
	std::string name;
	void Serialize(JsonNode node);
};

class Resource
{
public:
	Resource();
	Resource(uint UID);
	~Resource();
	virtual void CleanUp()=0;

	void SetUID(uint UID);

	uint GetUID()const;
	ResourceType GetType()const;

public:
	ResourceData resourceData;
};
#endif //__RESOURCE_H__
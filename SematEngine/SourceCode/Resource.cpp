#include "Config.h"
#include "Resource.h"


void ResourceData::Serialize(JsonNode node)
{
	node.AddNumber("UID", UID);
	node.AddNumber("Type", (double)type);
	node.AddString("Assets File", assetsFile.c_str());
	node.AddString("Library File", libraryFile.c_str());
	node.AddString("Name", name.c_str());
}

Resource::Resource()
{}

Resource::Resource(uint UID) 
{
	resourceData.UID = UID;
}

Resource::Resource(uint UID,ResourceType type)
{
	resourceData.UID = UID;
	resourceData.type = type;
}

Resource::~Resource()
{}

void Resource::SetUID(uint UID)
{
	this->resourceData.UID = UID;
}

UID Resource::GetUID()const
{
	return resourceData.UID;
}

ResourceType Resource::GetType()const
{
	return resourceData.type;
}
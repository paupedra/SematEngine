#include "Resource.h"
#include "Random.h"

#include "RTexture.h"

#include "ITexture.h"

#include "Dependecies/mmgr/mmgr.h"

#include <gl/GL.h>

RTexture::RTexture()
{
	resourceData.type = ResourceType::texture;
}

RTexture::RTexture(uint UID) : Resource(UID)
{
	resourceData.type = ResourceType::texture;
}

RTexture::~RTexture()
{

}

void RTexture::CleanUp()
{
	glDeleteTextures(1, (GLuint*)&id);
}

UID RTexture::GenerateCustomFile()
{
	std::string fileName = TEXTURES_PATH;
	fileName += std::to_string(resourceData.UID);
	fileName += ".texture";
	Importer::TextureImp::Save(this, fileName.c_str());
	return resourceData.UID;
}

void RTexture::SetId(uint id)
{
	this->id = id;
}

void RTexture::SetHeight(uint height)
{
	this->height = height;
}

void RTexture::SetWidth(uint width)
{
	this->width = width;
}

void RTexture::SetPath(const char* path)
{
	this->path = path;
}

uint RTexture::GetId() const
{
	return this->id;
}

uint RTexture::GetHeight() const
{
	return this->height;
}

uint RTexture::GetWidth() const
{
	return this->width;
}
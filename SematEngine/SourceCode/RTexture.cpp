#include "Resource.h"

#include "RTexture.h"

RTexture::RTexture()
{

}

RTexture::~RTexture()
{

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
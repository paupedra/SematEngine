#include "Resource.h"
#include "RMaterial.h"

RMaterial::RMaterial()
{

}

RMaterial::~RMaterial()
{

}

void RMaterial::SetId(uint id)
{
	this->id = id;
}

void RMaterial::SetHeight(uint height)
{
	this->height = height;
}

void RMaterial::SetWidth(uint width)
{
	this->width = width;
}

void RMaterial::SetPath(const char* path)
{
	this->path = path;
}

uint RMaterial::GetId() const
{
	return id;
}

uint RMaterial::GetHeight() const
{
	return height;
}

uint RMaterial::GetWidth() const
{
	return width;
}
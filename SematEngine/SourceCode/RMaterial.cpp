//#include "Resource.h"
#include "Color.h"

#include "RMaterial.h"
#include "RTexture.h"

RMaterial::RMaterial()
{
	texture = nullptr;
	color = nullptr;
}

RMaterial::~RMaterial()
{

}

void RMaterial::SetId(uint id)
{
	texture->id = id;
}

void RMaterial::SetHeight(uint height)
{
	texture->height = height;
}

void RMaterial::SetWidth(uint width)
{
	texture->width = width;
}

void RMaterial::SetPath(const char* path)
{
	texture->path = path;
}

void RMaterial::SetTexture(RTexture* texture)
{
	this->texture = texture;
}

uint RMaterial::GetId() const
{
	return texture->id;
}

uint RMaterial::GetHeight() const
{
	return texture->height;
}

uint RMaterial::GetWidth() const
{
	return texture->width;
}

RTexture* RMaterial::GetTexture() const
{
	return texture;
}

void RMaterial::SetColor(float r, float g, float b, float a)
{
	color->Set(r,g,b,a);
}
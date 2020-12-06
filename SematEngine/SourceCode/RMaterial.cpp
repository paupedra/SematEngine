//#include "Resource.h"
#include "Color.h"

#include "RMaterial.h"
#include "RTexture.h"

#include "Dependecies/mmgr/mmgr.h"

RMaterial::RMaterial()
{
	texture = nullptr;
	color = new Color(1,1,1,1);
}

RMaterial::RMaterial(RTexture* texture) : texture(texture)
{
	color = new Color(1, 1, 1, 1);
}

RMaterial::~RMaterial()
{

}

void RMaterial::CleanUp()
{
	if(texture != nullptr)
		texture->CleanUp();

	delete color;
	delete texture;
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

Color* RMaterial::GetColor()const
{
	return color;
}

void RMaterial::SetColor(Color color)
{
	this->color->Set(color);
}

void RMaterial::SetColor(float r, float g, float b , float a)
{
	this->color->Set(r, g, b, a);
}
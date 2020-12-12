//#include "Resource.h"


#include "RMaterial.h"
#include "RTexture.h"

#include "Dependecies/mmgr/mmgr.h"

RMaterial::RMaterial()
{
	resourceData.type = ResourceType::material;
	color = Color(1,1,1,1);
}


RMaterial::RMaterial(uint UID) : Resource(UID)
{
	resourceData.type = ResourceType::material;
	color = Color(1, 1, 1, 1);
}

RMaterial::~RMaterial()
{

}

void RMaterial::CleanUp()
{
	
}

Color RMaterial::GetColor()const
{
	return color;
}

void RMaterial::SetColor(Color color)
{
	this->color.Set(color);
}

void RMaterial::SetColor(float r, float g, float b , float a)
{
	this->color.Set(r, g, b, a);
}
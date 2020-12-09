//#include "Resource.h"


#include "RMaterial.h"
#include "RTexture.h"

#include "Dependecies/mmgr/mmgr.h"

RMaterial::RMaterial()
{
	color = Color(1,1,1,1);
}


RMaterial::RMaterial(uint UID) : Resource(UID)
{
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
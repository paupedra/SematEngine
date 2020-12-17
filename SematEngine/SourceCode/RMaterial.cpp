//#include "Resource.h"
#include "Application.h"
#include "Random.h"
#include "Config.h"

#include "MFileSystem.h"

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
	if (texture != nullptr)
		texture->CleanUp();
}

UID RMaterial::GenerateCustomFile(UID textureUID)
{
	//save mesh UID and color in Json
	UID ret = Random::GenerateUID();
	JsonNode root;
	root.AddNumber("Texture UID", textureUID);
	
	JsonArray colorJson = root.InitArray("Color");
	colorJson.AddNumber(color.r);
	colorJson.AddNumber(color.g);
	colorJson.AddNumber(color.b);
	colorJson.AddNumber(color.a);

	std::string fileName = MATERIALS_PATH;
	fileName += std::to_string(ret);
	fileName += MATERIAL_EXTENTION;

	char* buffer;
	uint size = root.Serialize(&buffer);

	App->fileSystem->Save(fileName.c_str(),buffer,size);

	return ret;
}

Color RMaterial::GetColor()const
{
	return color;
}
RTexture* RMaterial::GetTexture()const
{
	return texture;
}

void RMaterial::SetTexture(RTexture* texture)
{
	this->texture = texture;
}

void RMaterial::SetColor(Color color)
{
	this->color.Set(color);
}

void RMaterial::SetColor(float r, float g, float b , float a)
{
	this->color.Set(r, g, b, a);
}
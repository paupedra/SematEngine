#include "I_Texture.h"
#include "Globals.h"
#include "Dependecies/Devil/Include/ilut.h"
#include "Dependecies/Devil/Include/ilu.h"
#include "Dependecies/Devil/Include/il.h"

#pragma comment (lib, "Dependecies/Devil/libx86/DevIL.lib")
#pragma comment (lib, "Dependecies/Devil/libx86/ILU.lib")
#pragma comment (lib, "Dependecies/Devil/libx86/ILUT.lib")

void Importer::TextureImp::InitDevil()
{
	LOG("Initializing Devil")
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

Texture* Importer::TextureImp::Import(const char* path)
{
	Texture* newTexture = new Texture;
	uint i;
	
	ilGenImages(1,&i);
	ilBindImage(i);

	if (ilLoadImage(path))
	{
		ILinfo ImgInfo;
		iluGetImageInfo(&ImgInfo);

		if (ImgInfo.Origin == IL_ORIGIN_UPPER_LEFT)
			iluFlipImage();

		if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
		{
			newTexture->id = CreateTexture(ilGetData(), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT));
			newTexture->height = ilGetInteger(IL_IMAGE_HEIGHT);
			newTexture->width = ilGetInteger(IL_IMAGE_WIDTH);
			newTexture->path = path;

			LOG("Successfully loaded Texture from: %s", path);
		}
		else
		{
			LOG("(ERROR)Could not convert image %s", path);
		}
	}
	else
	{
		LOG("(ERROR) Error loading Image %s", path);
	}

	

	return newTexture;
}

uint Importer::TextureImp::CreateTexture(const void* data,uint width, uint height, uint format)
{
	uint id;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
		0, format, GL_UNSIGNED_BYTE, data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return id;
}
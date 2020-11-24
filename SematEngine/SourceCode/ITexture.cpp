#include "Globals.h"
#include "Resource.h"

#include "ITexture.h"

#include "RMaterial.h"

#include "Dependecies/Devil/Include/ilut.h"
#include "Dependecies/Devil/Include/ilu.h"
#include "Dependecies/Devil/Include/il.h"

#pragma comment (lib, "Dependecies/Devil/libx86/DevIL.lib")
#pragma comment (lib, "Dependecies/Devil/libx86/ILU.lib")
#pragma comment (lib, "Dependecies/Devil/libx86/ILUT.lib")

#include "Dependecies/mmgr/mmgr.h"

void Importer::TextureImp::InitDevil()
{
	LOG("Initializing Devil")
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

RMaterial* Importer::TextureImp::Import(const char* path)
{
	RMaterial* newMaterial = new RMaterial();
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
			newMaterial->SetId(CreateTexture(ilGetData(), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT)));
			newMaterial->SetHeight(ilGetInteger(IL_IMAGE_HEIGHT));
			newMaterial->SetWidth(ilGetInteger(IL_IMAGE_WIDTH));
			newMaterial->SetPath(path);

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

	return newMaterial;
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

uint64 Importer::TextureImp::Save(RMaterial* material)
{
	uint64 ret = 0;


	return ret;
}
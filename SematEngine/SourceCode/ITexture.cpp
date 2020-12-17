#include "Globals.h"
#include "Resource.h"
#include "Application.h"

#include "MFileSystem.h"

#include "ITexture.h"

#include "RTexture.h"
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

RTexture* Importer::TextureImp::Import(char* buffer, RTexture* newTexture, uint size,bool genBuffers)
{
	uint i;

	ilGenImages(1, &i);
	ilBindImage(i);

	if (ilLoadL(IL_TYPE_UNKNOWN, buffer, size))
	{
		ILinfo ImgInfo;
		iluGetImageInfo(&ImgInfo);

		if (ImgInfo.Origin == IL_ORIGIN_UPPER_LEFT)
			iluFlipImage();

		if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
		{
			if(genBuffers)
				newTexture->SetId(Importer::TextureImp::CreateTexture(ilGetData(), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT)));

			newTexture->SetHeight(ilGetInteger(IL_IMAGE_HEIGHT));
			newTexture->SetWidth(ilGetInteger(IL_IMAGE_WIDTH));
			//newTexture->SetPath(path);

			//LOG("Successfully loaded Texture from: %s", path);
		}
		else
		{
			//LOG("(ERROR)Could not convert image %s", path);
		}
	}
	else
	{
		//LOG("(ERROR) Error loading Image %s", path);
	}

	return newTexture;
}

RTexture Importer::TextureImp::Import(const char* path)
{
	char* buffer;
	uint size = App->fileSystem->Load(path, &buffer);
	
	RTexture newTexture;
	uint i;
	
	ilGenImages(1,&i);
	ilBindImage(i);

	if (ilLoadL(IL_TYPE_UNKNOWN,buffer,size))
	{
		ILinfo ImgInfo;
		iluGetImageInfo(&ImgInfo);

		if (ImgInfo.Origin == IL_ORIGIN_UPPER_LEFT)
			iluFlipImage();

		if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
		{
			//newTexture->SetId(Importer::TextureImp::CreateTexture(ilGetData(), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT)));
			newTexture.SetHeight(ilGetInteger(IL_IMAGE_HEIGHT));
			newTexture.SetWidth(ilGetInteger(IL_IMAGE_WIDTH));
			newTexture.SetPath(path);

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
	RELEASE_ARRAY(buffer);

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

uint64 Importer::TextureImp::Save(RTexture* material,const char* path)
{
	uint64 ret = 0;

	ILuint size;
	ILubyte* data;

	char* fileBuffer;

	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
	size = ilSaveL(IL_DDS, nullptr, 0); // Get the size of the data buffer

	if (size > 0) {
		data = new ILubyte[size]; // allocate data buffer
		if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
		{
			fileBuffer = (char*)data;

			App->fileSystem->Save(path,fileBuffer,size);
		}
	}


	delete[] fileBuffer;

	return ret;
}
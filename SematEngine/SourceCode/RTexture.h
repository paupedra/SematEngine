#ifndef __RESOURCETEXTURE_H__
#define __RESOURCETEXTURE_H__
#include "Dependecies/MathGeoLib/include/MathGeoLib.h"

class Resource;

class RTexture : public Resource
{
public:

	RTexture();
	RTexture(uint UID);
	~RTexture();

	void CleanUp() override;

	enum Buffers
	{
		index,
		vertex,
		normal,
		texture,
		maxBuffers
	};

	void SetId(uint id); //OpenGl id
	void SetHeight(uint height);
	void SetWidth(uint width);
	void SetPath(const char* path);
	//void SetTexture(RTexture* texture);

	uint GetId() const;
	uint GetHeight() const;
	uint GetWidth() const;
	//RTexture* GetTexture()const;

public:
	
	uint width = 0;
	uint height = 0;

	uint id = 0; //Open gl texture ID

	bool usesTexture = false;

	const char* path = "";
};
#endif //__RESOURCETEXTURE_H__
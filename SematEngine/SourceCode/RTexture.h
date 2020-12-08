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

	void SetId(uint id);
	void SetHeight(uint height);
	void SetWidth(uint width);
	void SetPath(const char* path);

	uint GetId() const;
	uint GetHeight() const;
	uint GetWidth() const;

	enum Buffers
	{
		index,
		vertex,
		normal,
		texture,
		maxBuffers
	};

public:
	
	uint width = 0;
	uint height = 0;

	uint id = 0; //Open gl texture ID

	const char* path = nullptr;
};
#endif //__RESOURCETEXTURE_H__
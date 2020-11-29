#ifndef __RESOURCEMATERIAL_H__
#define __RESOURCEMATERIAL_H__

#include "Resource.h"
class RTexture;
class Color;

class RMaterial : public Resource
{
public:

	RMaterial();
	RMaterial(RTexture* texture);
	~RMaterial();

	void SetId(uint id);
	void SetHeight(uint height);
	void SetWidth(uint width);
	void SetPath(const char* path);
	void SetTexture(RTexture* texture);

	uint GetId() const;
	uint GetHeight() const;
	uint GetWidth() const;
	RTexture* GetTexture()const;

	void SetColor(float r = 0, float g = 0, float b = 0, float a = 1.f);
	
private:
	RTexture* texture;
	Color* color;

public:
	bool drawTexture = false;
}; 
#endif //__RESOURCEMATERIAL_H__
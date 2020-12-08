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
	RMaterial(uint UID);
	~RMaterial();

	void CleanUp() override;

	void SetId(uint id);
	void SetHeight(uint height);
	void SetWidth(uint width);
	void SetPath(const char* path);
	void SetTexture(RTexture* texture);

	uint GetId() const;
	uint GetHeight() const;
	uint GetWidth() const;
	RTexture* GetTexture()const;
	Color* GetColor()const;

	void SetColor(float r = 1, float g = 1, float b = 1, float a = 1.f);
	void SetColor(Color color);

private:
	RTexture* texture;
	Color* color;

public:
	bool drawTexture = false;
}; 
#endif //__RESOURCEMATERIAL_H__
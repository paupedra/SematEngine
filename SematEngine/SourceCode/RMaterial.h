#ifndef __RESOURCEMATERIAL_H__
#define __RESOURCEMATERIAL_H__

#include "Resource.h"
class RTexture;
#include "Color.h"

class RMaterial : public Resource
{
public:

	RMaterial();
	RMaterial(uint UID);
	~RMaterial();

	void CleanUp() override;


	Color GetColor()const;

	void SetColor(float r = 1, float g = 1, float b = 1, float a = 1.f);
	void SetColor(Color color);

private:
	
	Color color;
}; 
#endif //__RESOURCEMATERIAL_H__
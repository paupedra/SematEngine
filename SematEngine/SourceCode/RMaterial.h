#ifndef __RESOURCEMATERIAL_H__
#define __RESOURCEMATERIAL_H__
class Resource;

class RMaterial : public Resource
{
public:

	RMaterial();
	~RMaterial();

	void SetId(uint id);
	void SetHeight(uint height);
	void SetWidth(uint width);
	void SetPath(const char* path);

	uint GetId() const;
	uint GetHeight() const;
	uint GetWidth() const;
	
private:

	uint width = 0;
	uint height = 0;

	uint id = 0; //Open gl texture ID

	const char* path = nullptr;

}; 
#endif //__RESOURCEMATERIAL_H__
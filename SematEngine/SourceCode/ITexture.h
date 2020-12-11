#include "Globals.h"

class RMaterial;
class RTexture;

namespace Importer
{
	namespace TextureImp
	{
		void InitDevil();
		RTexture* Import(char* buffer,RTexture* texture,uint size); //Takes loaded buffer and stores info in resource
		RTexture* Import(const char* path);
		uint CreateTexture(const void* data, uint width, uint height, uint format); //Binds texture to gl
		uint64 Save(RTexture* material,const char* path);
		//void Load()
	}
}

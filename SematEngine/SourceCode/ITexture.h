#include "Globals.h"

class RMaterial;
class RTexture;
struct aiMaterial;

namespace Importer
{
	namespace TextureImp
	{
		void InitDevil();
		RTexture* Import(char* buffer,RTexture* texture,uint size, bool save = true); //Takes loaded buffer and stores info in resource
		RTexture* Import(const char* path); //Takes a texture path and loads it in a buffer
		uint CreateTexture(const void* data, uint width, uint height, uint format); //Binds texture to gl
		uint64 Save(RTexture* material,const char* path);
		//void Load()

		
	}
}

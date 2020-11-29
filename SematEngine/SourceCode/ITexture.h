#include "Globals.h"

class RMaterial;
class RTexture;

namespace Importer
{
	namespace TextureImp
	{
		void InitDevil();
		RTexture* Import(const char* path);
		uint CreateTexture(const void* data, uint width, uint height, uint format); //Binds texture to gl
		uint64 Save(RMaterial* material);
		//void Load()
	}
}

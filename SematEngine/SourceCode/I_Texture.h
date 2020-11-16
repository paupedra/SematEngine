#include "Globals.h"

struct Texture
{
	uint width;
	uint height;
	
	uint id;
	const char* path;
};

namespace Importer
{
	namespace TextureImp
	{
		void InitDevil();
		Texture* Import(const char* path);
		uint CreateTexture(const void* data, uint width, uint height, uint format); //Binds texture to gl
		uint64 Save(Texture* texture);
		//void Load()
	}
}

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
		Texture Import(const char* path);
		uint CreateTexture(const void* data, uint width, uint height, uint format);
		void InitDevil();
	}
}

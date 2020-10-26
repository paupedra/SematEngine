#ifndef __MESHIMP__
#define __MESHIMG__

#include "Globals.h"

struct Mesh
{
	Mesh() {};

	enum Buffers
	{
		index,
		vertex,
		normal,
		maxBuffers
	};

	uint buffersId[maxBuffers];
	uint buffersSize[maxBuffers];

	uint* indices = nullptr;
	float* vertices = nullptr;
	float* normals = nullptr;
};

namespace Importer
{
	namespace MeshImp
	{
		void Import(const char* file);
	}
}

#endif //__MESHIMP__
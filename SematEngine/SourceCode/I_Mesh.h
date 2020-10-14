#ifndef __MODULE_MESHES_H__
#define __MODULE_MESHES_H__

#include "Globals.h"

struct Mesh
{
	Mesh() {};

	enum Buffers
	{
		index,
		vertex,
		maxBuffers
	};

	uint buffersId[maxBuffers];
	uint buffersSize[maxBuffers];

	uint* indices = nullptr;
	float* vertices = nullptr;

	//uint buffer;

};

namespace Importer
{
	namespace MeshImp
	{
		void Import(const char* file);
	}
}

#endif //__MODULE_MESHES_H__
#ifndef __MESHIMP__
#define __MESHIMG__

#include "Globals.h"
#include <vector>

struct Mesh
{
	Mesh() {};

	enum Buffers
	{
		index,
		vertex,
		normal,
		texture,
		maxBuffers
	};

	uint buffersId[maxBuffers];
	uint buffersSize[maxBuffers];

	uint* indices = nullptr;
	float* vertices = nullptr;
	float* normals = nullptr;
	float* textureCoords = nullptr;

	const char* path;
};

namespace Importer
{
	namespace MeshImporter
	{
		std::vector<Mesh*> Import(const char* file);

		void Save(const Mesh mesh);

		void Load(const char* fileBuffer, Mesh* mesh);

	}
}

#endif //__MESHIMP__
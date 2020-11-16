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

		uint64 Save(const Mesh mesh);	//Store mesh info into custom format file

		void Load(const char* fileBuffer, Mesh* mesh); //Load buffer from custom format file and store into mesh

	}
}

#endif //__MESHIMP__
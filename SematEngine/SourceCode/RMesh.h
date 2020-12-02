#ifndef __RESOURCEMESH_H__
#define __RESOURCEMESH_H__
#include "Dependecies/MathGeoLib/include/MathGeoLib.h"

class Resource;

class RMesh : public Resource
{
public:

	RMesh();
	~RMesh();

	enum Buffers
	{
		index,
		vertex,
		normal,
		texture,
		maxBuffers
	};

public:
	uint buffersId[RMesh::maxBuffers] = {};
	uint buffersSize[RMesh::maxBuffers] = {};

	uint* indices = nullptr;
	float* vertices = nullptr;
	float* normals = nullptr;
	float* textureCoords = nullptr;


	const char* path;

	AABB aabb;
}; 
#endif //__RESOURCEMESH_H__
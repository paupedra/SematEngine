#ifndef __RESOURCEMESH_H__
#define __RESOURCEMESH_H__
#include "Dependecies/MathGeoLib/include/MathGeoLib.h"

class Resource;

class RMesh : public Resource
{
public:

	RMesh();
	RMesh(uint UID);
	~RMesh();

	void CleanUp() override;

	UID GenerateCustomFile();

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

	char name[30] = "Default";

	AABB aabb;
}; 
#endif //__RESOURCEMESH_H__
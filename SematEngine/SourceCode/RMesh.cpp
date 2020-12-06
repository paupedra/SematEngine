#include "Resource.h"
#include "RMesh.h"

#include "Dependecies/mmgr/mmgr.h"

RMesh::RMesh()
{

}

RMesh::~RMesh()
{

}

void RMesh::CleanUp()
{
	delete[] indices;
	delete[] normals;
	delete[] textureCoords;
	delete[] vertices;
	//delete[] path;
}
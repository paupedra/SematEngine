#include "Resource.h"
#include "RMesh.h"

#include "Dependecies/mmgr/mmgr.h"


#include "Dependecies/Glew/include/glew.h"
#include <gl/GL.h>

RMesh::RMesh()
{
	resourceData.type = ResourceType::mesh;
}

RMesh::RMesh(uint UID) : Resource(UID)
{
	resourceData.type = ResourceType::mesh;
}

RMesh::~RMesh()
{

}

void RMesh::CleanUp()
{
	glDeleteBuffers(1, (GLuint*)&buffersId[RMesh::index]);
	glDeleteBuffers(1, (GLuint*)&buffersId[RMesh::normal]);
	glDeleteBuffers(1, (GLuint*)&buffersId[RMesh::texture]);
	glDeleteBuffers(1, (GLuint*)&buffersId[RMesh::vertex]);
	delete[] indices;
	delete[] normals;
	delete[] textureCoords;
	delete[] vertices;
	//delete[] path;
}
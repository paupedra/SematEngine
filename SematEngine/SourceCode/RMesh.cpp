#include "Application.h"
#include "Resource.h"
#include "RMesh.h"
#include "Random.h"

#include "MFileSystem.h"

#include "IMesh.h"

//#include "Dependecies/mmgr/mmgr.h"

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
	RELEASE_ARRAY(indices);
	RELEASE_ARRAY(normals);
	RELEASE_ARRAY(textureCoords);
	RELEASE_ARRAY(vertices);
	//delete[] path;
}

UID RMesh::GenerateCustomFile()
{
	std::string fileName = MESHES_PATH + std::to_string(resourceData.UID) + MESH_EXTENTION;
	Importer::MeshImporter::Save(*this,fileName.c_str() );
	return 0;
}
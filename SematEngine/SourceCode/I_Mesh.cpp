#include "I_Mesh.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

#include "Dependecies/Glew/include/glew.h"
#include "Dependecies/SDL/include/SDL_opengl.h" 
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Dependecies/Assimp/include/mesh.h"
#include "Dependecies/Assimp/include/cimport.h"
#include "Dependecies/Assimp/include/scene.h"
#include "Dependecies/Assimp/include/postprocess.h"

#pragma comment (lib, "Dependecies/Assimp/libx86/assimp.lib")

void Importer::MeshImp::Import(const char* file)
{
	const aiScene* scene = aiImportFile(file, aiProcessPreset_TargetRealtime_MaxQuality);

    if (scene != nullptr && scene->HasMeshes())
    {
        // Use scene->mNumMeshes to iterate on scene->mMeshes array

        for (int i = 0; i < scene->mNumMeshes; i++)
        {
            Mesh* newMesh = new Mesh();

            newMesh->buffersId[Mesh::vertex] = 0;
            newMesh->buffersId[Mesh::index] = 0;
            

            newMesh->buffersSize[Mesh::vertex] = scene->mMeshes[i]->mNumVertices;
            newMesh->vertices = new float[newMesh->buffersSize[Mesh::vertex] * 3];
            memcpy(newMesh->vertices, scene->mMeshes[i]->mVertices, sizeof(float) * newMesh->buffersSize[Mesh::vertex] * 3);
            LOG("New mesh with %d vertices", newMesh->buffersSize[Mesh::vertex]);

            if (scene->mMeshes[i]->HasFaces())
            {
                newMesh->buffersSize[Mesh::index] = scene->mMeshes[i]->mNumFaces * 3;
                newMesh->indices = new uint[newMesh->buffersSize[Mesh::index]];
                for (uint f = 0; f < scene->mMeshes[i]->mNumFaces; ++f)
                {
                    if (scene->mMeshes[i]->mFaces[f].mNumIndices != 3)
                    {
                        LOG("WARNING, geometery face with != 3 indices!");
                    }
                    else
                    {
                        memcpy(&newMesh->indices[f * 3], scene->mMeshes[i]->mFaces[f].mIndices, 3 * sizeof(uint));
                    }

                }
            }
            //Vertex buffer
            glGenBuffers(1, (GLuint*)&(newMesh->buffersId[Mesh::vertex]));
            glBindBuffer(GL_ARRAY_BUFFER, newMesh->buffersId[Mesh::vertex]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * newMesh->buffersSize[Mesh::vertex] * 3, newMesh->vertices, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            if (newMesh->indices != nullptr)
            {
                //Index buffer
                glGenBuffers(1, (GLuint*)&(newMesh->buffersId[Mesh::index]));
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh->buffersId[Mesh::vertex]);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * newMesh->buffersSize[Mesh::index], newMesh->indices, GL_STATIC_DRAW);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }

            App->renderer3D->meshes.push_back(newMesh);
        }

        aiReleaseImport(scene);
    }

    else
        LOG("Error loading scene %s", file);
}
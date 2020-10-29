#include "I_Mesh.h"

#include "Application.h"
#include "ModuleRenderer3D.h"

#include "Dependecies/Assimp/include/mesh.h"
#include "Dependecies/Assimp/include/cimport.h"
#include "Dependecies/Assimp/include/scene.h"
#include "Dependecies/Assimp/include/postprocess.h"

#pragma comment (lib, "Dependecies/Assimp/libx86/assimp.lib")

#include "Dependecies/mmgr/mmgr.h"

std::vector<Mesh*> Importer::MeshImp::Import(const char* file)
{
	const aiScene* scene = aiImportFile(file, aiProcessPreset_TargetRealtime_MaxQuality);
    std::vector<Mesh*> ret;

    if (scene != nullptr && scene->HasMeshes())
    {
        // Use scene->mNumMeshes to iterate on scene->mMeshes array
        for (int i = 0; i < scene->mNumMeshes; i++)
        {
            Mesh* newMesh = new Mesh();
            
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

            if (scene->mMeshes[i]->HasNormals())
            {
                newMesh->buffersSize[Mesh::normal] = scene->mMeshes[i]->mNumVertices;
                newMesh->normals = new float[newMesh->buffersSize[Mesh::normal] * 3];
                memcpy(newMesh->normals, scene->mMeshes[i]->mNormals, sizeof(float) * newMesh->buffersSize[Mesh::normal] * 3);
            }

            if (scene->mMeshes[i]->HasTextureCoords(0))
            {
                newMesh->buffersSize[Mesh::texture] = scene->mMeshes[i]->mNumVertices;
                newMesh->textureCoords = new float[scene->mMeshes[i]->mNumVertices * 2];

                for (int j = 0; j < newMesh->buffersSize[Mesh::texture]; j++)
                {
                    newMesh->textureCoords[j * 2] = scene->mMeshes[i]->mTextureCoords[0][j].x;
                    newMesh->textureCoords[j * 2 + 1] = scene->mMeshes[i]->mTextureCoords[0][j].y;
                }
            }

            App->renderer3D->GenerateBuffers(newMesh);

            ret.push_back(newMesh);
        }

        aiReleaseImport(scene);
    }
    else
    {
        LOG("Error loading scene %s", file);
    }
    return ret;
}
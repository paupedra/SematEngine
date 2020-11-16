#include "I_Mesh.h"

#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleFileSystem.h"

#include "Timer.h"

#include "Dependecies/Assimp/include/mesh.h"
#include "Dependecies/Assimp/include/cimport.h"
#include "Dependecies/Assimp/include/scene.h"
#include "Dependecies/Assimp/include/postprocess.h"

#pragma comment (lib, "Dependecies/Assimp/libx86/assimp.lib")

#include "Dependecies/mmgr/mmgr.h"

std::vector<Mesh*> Importer::MeshImporter::Import(const char* file)
{
    Timer* timeImporting = new Timer();

    //aiImportFileFromMemory
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

            if (scene->HasTextures())
            {
                aiMaterial* newMaterial;
                uint materialIndex = scene->mMeshes[i]->mMaterialIndex;
                //newMaterial = scene->mMaterials[materialIndex]->GetTexture();
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

            ////Save and load trying
            //uint64 id = Importer::MeshImporter::Save(*newMesh);

            ////Load shit
            //std::string filename = "Library/Meshes/";

            //char tmp[5];
            //sprintf(tmp, "%d", id); //id into char
            //filename += tmp;
            //LOG("Loading filename: %s", filename.c_str());
            //char* buffer;

            //App->fileSystem->Load(filename.c_str(), &buffer);

            //Mesh* newNewMesh = new Mesh();

            //Importer::MeshImporter::Load(buffer,newNewMesh);

            //newMesh->path = file;

            //newMesh->buffersSize[0] = newNewMesh->buffersSize[0];
            //newMesh->buffersSize[1] = newNewMesh->buffersSize[1];
            //newMesh->buffersSize[2] = newNewMesh->buffersSize[2];
            //newMesh->buffersSize[3] = newNewMesh->buffersSize[3];
            ////

            App->renderer3D->GenerateBuffers(newMesh);

            ret.push_back(newMesh);
        }
        aiReleaseImport(scene);
        LOG("Time spent Importing Mesh: %d ms", timeImporting->Read());
    }
    else
    {
        LOG("(ERROR) Error loading scene %s", file);
    }
    return ret;
}

uint64 Importer::MeshImporter::Save(const Mesh mesh)
{
    Timer* timeSaving = new Timer();

    uint sizes[4] = { mesh.buffersSize[Mesh::index] ,mesh.buffersSize[Mesh::vertex], mesh.buffersSize[Mesh::normal], mesh.buffersSize[Mesh::texture] };

    uint size = sizeof(sizes) + sizeof(uint) * mesh.buffersSize[Mesh::index] + sizeof(float) * mesh.buffersSize[Mesh::vertex] * 3
        + sizeof(float) * mesh.buffersSize[Mesh::normal] * 3 + sizeof(float) * mesh.buffersSize[Mesh::texture] * 2;

    char* fileBuffer = new char[size];
    char* cursor = fileBuffer;

    uint bytes = sizeof(sizes);
    memcpy(cursor, sizes, bytes);
    cursor += bytes;

    //Indices
    bytes = sizeof(uint) * mesh.buffersSize[Mesh::index];
    memcpy(cursor, mesh.indices, bytes);
    cursor += bytes;

    //vertices
    bytes = sizeof(float) * mesh.buffersSize[Mesh::vertex] * 3;
    memcpy(cursor, mesh.vertices, bytes);
    cursor += bytes;

    //normals
    bytes = sizeof(float) * mesh.buffersSize[Mesh::normal] * 3;
    memcpy(cursor, mesh.normals, bytes);
    cursor += bytes;

    //TextureCoords
    bytes = sizeof(float) * mesh.buffersSize[Mesh::texture] * 2;
    memcpy(cursor, mesh.textureCoords, bytes);
    cursor += bytes;

    //Write buffer into file
    int id = rand() % 50;
    char tmp[5];
    sprintf(tmp, "%d", id);
    std::string fileName = "Library/Meshes/";
    fileName += tmp;

    LOG("Saved mesh in id: %s", fileName.c_str());

    App->fileSystem->Save(fileName.c_str(), fileBuffer, size);

    LOG("Time spent saving Mesh: %d ms", timeSaving->Read());

    return id;
}

void Importer::MeshImporter::Load(const char* fileBuffer, Mesh* mesh)
{
    Timer* timeLoading = new Timer();

    const char* cursor = fileBuffer;

    uint sizes[4];
    uint bytes = sizeof(sizes);
    memcpy(sizes, cursor, bytes);
    cursor += bytes;

    mesh->buffersSize[Mesh::index] = sizes[0];
    mesh->buffersSize[Mesh::vertex] = sizes[1];
    mesh->buffersSize[Mesh::normal] = sizes[2];
    mesh->buffersSize[Mesh::texture] = sizes[3];

    //Indices
    bytes = sizeof(uint) * mesh->buffersSize[Mesh::index];
    mesh->indices = new uint[mesh->buffersSize[Mesh::index]];
    memcpy(mesh->indices, cursor, bytes);
    cursor += bytes;

    //Vertices
    bytes = sizeof(float) * mesh->buffersSize[Mesh::vertex] * 3;
    mesh->vertices = new float[mesh->buffersSize[Mesh::vertex] * 3];
    memcpy(mesh->vertices, cursor, bytes);
    cursor += bytes;

    //Normals
    bytes = sizeof(float) * mesh->buffersSize[Mesh::normal] * 3;
    mesh->normals = new float[mesh->buffersSize[Mesh::normal] * 3];
    memcpy(mesh->normals, cursor, bytes);
    cursor += bytes;

    for (int i = 0; i < mesh->buffersSize[Mesh::normal] * 3; i++)
    {
        uint bruh = mesh->normals[i];
    }

    //Texture Coords
    bytes = sizeof(float) * mesh->buffersSize[Mesh::texture] * 2;
    mesh->textureCoords = new float[mesh->buffersSize[Mesh::texture] * 2];
    memcpy(mesh->textureCoords, cursor, bytes);
    cursor += bytes;

    LOG("Time spent loading Mesh: %d ms", timeLoading->Read());
}
#include "Application.h"
#include "Timer.h"
#include "Resource.h"

#include "MRenderer3D.h"
#include "MFileSystem.h"

#include "IMesh.h"

#include "RMesh.h"

#include "Dependecies/Assimp/include/mesh.h"
#include "Dependecies/Assimp/include/cimport.h"
#include "Dependecies/Assimp/include/scene.h"
#include "Dependecies/Assimp/include/postprocess.h"

#pragma comment (lib, "Dependecies/Assimp/libx86/assimp.lib")

#include "Dependecies/mmgr/mmgr.h"

std::vector<RMesh*> Importer::MeshImporter::Import(const char* file)
{
    Timer* timeImporting = new Timer();

    //aiImportFileFromMemory
	const aiScene* scene = aiImportFile(file, aiProcessPreset_TargetRealtime_MaxQuality);
    std::vector<RMesh*> ret;

    if (scene != nullptr && scene->HasMeshes())
    {
        // Use scene->mNumMeshes to iterate on scene->mMeshes array
        for (int i = 0; i < scene->mNumMeshes; i++)
        {
            RMesh* newMesh = new RMesh();
            
            newMesh->buffersSize[RMesh::vertex] = scene->mMeshes[i]->mNumVertices;
            newMesh->vertices = new float[newMesh->buffersSize[RMesh::vertex] * 3];
            memcpy(newMesh->vertices, scene->mMeshes[i]->mVertices, sizeof(float) * newMesh->buffersSize[RMesh::vertex] * 3);
            LOG("New mesh with %d vertices", newMesh->buffersSize[RMesh::vertex]);

            if (scene->mMeshes[i]->HasFaces())
            {
                newMesh->buffersSize[RMesh::index] = scene->mMeshes[i]->mNumFaces * 3;
                newMesh->indices = new uint[newMesh->buffersSize[RMesh::index]];
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
                newMesh->buffersSize[RMesh::normal] = scene->mMeshes[i]->mNumVertices;
                newMesh->normals = new float[newMesh->buffersSize[RMesh::normal] * 3];
                memcpy(newMesh->normals, scene->mMeshes[i]->mNormals, sizeof(float) * newMesh->buffersSize[RMesh::normal] * 3);
            }

            if (scene->mMeshes[i]->HasTextureCoords(0))
            {
                newMesh->buffersSize[RMesh::texture] = scene->mMeshes[i]->mNumVertices;
                newMesh->textureCoords = new float[scene->mMeshes[i]->mNumVertices * 2];

                for (int j = 0; j < newMesh->buffersSize[RMesh::texture]; j++)
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

    delete timeImporting;

    return ret;
}

void Importer::MeshImporter::LoadNodeMesh(const aiScene* scene, const aiNode* node, std::vector<RMesh*> &meshes)
{
    for (int i = 0; i < node->mNumMeshes; i++)
    {
        RMesh* newMesh = new RMesh();

        newMesh->buffersSize[RMesh::vertex] = scene->mMeshes[node->mMeshes[i]]->mNumVertices;
        newMesh->vertices = new float[newMesh->buffersSize[RMesh::vertex] * 3];
        memcpy(newMesh->vertices, scene->mMeshes[node->mMeshes[i]]->mVertices, sizeof(float) * newMesh->buffersSize[RMesh::vertex] * 3);
        LOG("New mesh with %d vertices", newMesh->buffersSize[RMesh::vertex]);

        if (scene->mMeshes[node->mMeshes[i]]->HasFaces())
        {
            newMesh->buffersSize[RMesh::index] = scene->mMeshes[node->mMeshes[i]]->mNumFaces * 3;
            newMesh->indices = new uint[newMesh->buffersSize[RMesh::index]];

            for (uint f = 0; f < scene->mMeshes[node->mMeshes[i]]->mNumFaces; ++f)
            {
                if (scene->mMeshes[node->mMeshes[i]]->mFaces[f].mNumIndices != 3)
                {
                    LOG("WARNING, geometery face with != 3 indices!"); //Problems with != 3 faces
                }
                else
                {
                    memcpy(&newMesh->indices[f * 3], scene->mMeshes[node->mMeshes[i]]->mFaces[f].mIndices, 3 * sizeof(uint));
                }
            }
        }

        if (scene->mMeshes[node->mMeshes[i]]->HasNormals())
        {
            newMesh->buffersSize[RMesh::normal] = scene->mMeshes[node->mMeshes[i]]->mNumVertices;
            newMesh->normals = new float[newMesh->buffersSize[RMesh::normal] * 3];
            memcpy(newMesh->normals, scene->mMeshes[node->mMeshes[i]]->mNormals, sizeof(float) * newMesh->buffersSize[RMesh::normal] * 3);
        }

        if (scene->mMeshes[node->mMeshes[i]]->HasTextureCoords(0))
        {
            newMesh->buffersSize[RMesh::texture] = scene->mMeshes[node->mMeshes[i]]->mNumVertices;
            newMesh->textureCoords = new float[scene->mMeshes[node->mMeshes[i]]->mNumVertices * 2];

            for (int j = 0; j < newMesh->buffersSize[RMesh::texture]; j++)
            {
                newMesh->textureCoords[j * 2] = scene->mMeshes[node->mMeshes[i]]->mTextureCoords[0][j].x;
                newMesh->textureCoords[j * 2 + 1] = scene->mMeshes[node->mMeshes[i]]->mTextureCoords[0][j].y;
            }
        }


         App->renderer3D->GenerateBuffers(newMesh); //Crashes
         meshes.push_back(newMesh);

    }
}

uint64 Importer::MeshImporter::Save(const RMesh mesh)
{
    Timer* timeSaving = new Timer();

    uint sizes[4] = { mesh.buffersSize[RMesh::index] ,mesh.buffersSize[RMesh::vertex], mesh.buffersSize[RMesh::normal], mesh.buffersSize[RMesh::texture] };

    uint size = sizeof(sizes) + sizeof(uint) * mesh.buffersSize[RMesh::index] + sizeof(float) * mesh.buffersSize[RMesh::vertex] * 3
        + sizeof(float) * mesh.buffersSize[RMesh::normal] * 3 + sizeof(float) * mesh.buffersSize[RMesh::texture] * 2;

    char* fileBuffer = new char[size];
    char* cursor = fileBuffer;

    uint bytes = sizeof(sizes);
    memcpy(cursor, sizes, bytes);
    cursor += bytes;

    //Indices
    bytes = sizeof(uint) * mesh.buffersSize[RMesh::index];
    memcpy(cursor, mesh.indices, bytes);
    cursor += bytes;

    //vertices
    bytes = sizeof(float) * mesh.buffersSize[RMesh::vertex] * 3;
    memcpy(cursor, mesh.vertices, bytes);
    cursor += bytes;

    //normals
    bytes = sizeof(float) * mesh.buffersSize[RMesh::normal] * 3;
    memcpy(cursor, mesh.normals, bytes);
    cursor += bytes;

    //TextureCoords
    bytes = sizeof(float) * mesh.buffersSize[RMesh::texture] * 2;
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

void Importer::MeshImporter::Load(const char* fileBuffer, RMesh* mesh)
{
    Timer* timeLoading = new Timer();

    const char* cursor = fileBuffer;

    uint sizes[4];
    uint bytes = sizeof(sizes);
    memcpy(sizes, cursor, bytes);
    cursor += bytes;

    mesh->buffersSize[RMesh::index] = sizes[0];
    mesh->buffersSize[RMesh::vertex] = sizes[1];
    mesh->buffersSize[RMesh::normal] = sizes[2];
    mesh->buffersSize[RMesh::texture] = sizes[3];

    //Indices
    bytes = sizeof(uint) * mesh->buffersSize[RMesh::index];
    mesh->indices = new uint[mesh->buffersSize[RMesh::index]];
    memcpy(mesh->indices, cursor, bytes);
    cursor += bytes;

    //Vertices
    bytes = sizeof(float) * mesh->buffersSize[RMesh::vertex] * 3;
    mesh->vertices = new float[mesh->buffersSize[RMesh::vertex] * 3];
    memcpy(mesh->vertices, cursor, bytes);
    cursor += bytes;

    //Normals
    bytes = sizeof(float) * mesh->buffersSize[RMesh::normal] * 3;
    mesh->normals = new float[mesh->buffersSize[RMesh::normal] * 3];
    memcpy(mesh->normals, cursor, bytes);
    cursor += bytes;

    //Texture Coords
    bytes = sizeof(float) * mesh->buffersSize[RMesh::texture] * 2;
    mesh->textureCoords = new float[mesh->buffersSize[RMesh::texture] * 2];
    memcpy(mesh->textureCoords, cursor, bytes);
    cursor += bytes;

    LOG("Time spent loading Mesh: %d ms", timeLoading->Read());
}
#include "Application.h"
#include "Resource.h"

#include "MResourceManager.h"
#include "MFileSystem.h"

#include "RMaterial.h"
#include "RTexture.h"

#include "IMaterial.h"
#include "ITexture.h"

#include "Dependecies/Assimp/include/mesh.h"
#include "Dependecies/Assimp/include/cimport.h"
#include "Dependecies/Assimp/include/scene.h"
#include "Dependecies/Assimp/include/postprocess.h"


uint Importer::MaterialImporter::Import(aiMaterial material)
{
	uint textureUid = 0;
	RMaterial newMaterial;

	aiColor4D color;
	if (material.Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
	{
		newMaterial.SetColor(color.r, color.g, color.b, color.a);
	}

	aiString path;

	if (material.GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
	{
		std::string file;
		std::string extention;
		App->fileSystem->SplitFilePath(path.C_Str(), nullptr, &file, &extention);
		file += "." + extention;
		std::string finalPath = "Assets/Textures/";
		finalPath += file;
		std::string metaPath = finalPath + ".meta";
		textureUid = App->resourceManager->GetResourceUID(metaPath.c_str()); //Check if there is a meta of this image and if so get that id and push it

		if (textureUid == 0)
		{
			App->resourceManager->ImportFile(finalPath.c_str(), ResourceType::texture); //Import texture
		}
	}

	newMaterial.GenerateCustomFile(textureUid); //Create cff with the uid of the texture and the color

	return newMaterial.GetUID();
}

void Importer::MaterialImporter::ImportAllMaterialsInScene(const aiScene* scene, std::vector<uint>& material)
{
	for (int i = 0 ; i < scene->mNumMaterials ; i++)
	{
		material.push_back(Import(*scene->mMaterials[i]));
	}
}
#include "Application.h"
#include "GameObject.h"
#include "Resource.h"
#include "Config.h"
#include "Component.h"
#include "Resource.h"

#include "MFileSystem.h"
#include "MScene.h"
#include "MResourceManager.h"

#include "CTransform.h"
#include "CMesh.h"
#include "CMaterial.h"

#include "IScene.h"
#include "IMesh.h"
#include "ITexture.h"

#include "RMesh.h"
#include "RMaterial.h"

#include "Dependecies/Assimp/include/mesh.h"
#include "Dependecies/Assimp/include/cimport.h"
#include "Dependecies/Assimp/include/scene.h"
#include "Dependecies/Assimp/include/postprocess.h"

#include "Dependecies/MathGeoLib/include/MathGeoLib.h"
#include "Dependecies/mmgr/mmgr.h"

void Importer::SceneImporter::Import(const char* file) //Load buffer with .fbx scene
{
	//Check if we have this .fbx in library?

	char* buffer;
	uint byteSize = App->fileSystem->Load(file, &buffer);

	LOG("Began importing: %s", file);

	if (byteSize > 0)
	{
		const aiScene* scene = aiImportFileFromMemory(buffer, byteSize, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);
		const aiNode* rootNode = scene->mRootNode;

		ProcessAiNode(scene,rootNode,App->scene->rootObject,file); //Process node tree
		int i = 0;
		for (; i < scene->mNumMeshes; i++)
		{
			LOG("Mesh has %d whatever", scene->mMeshes[i]->mNumVertices);
		}
		LOG("There are %d meshes in this .fbx file" , i);

		LOG("Finished importing: %s", file);
	}
	else
	{
		LOG("(ERROR) Could not load .fbx file");
	}
	delete[] buffer;
}

void Importer::SceneImporter::ProcessAiNode(const aiScene* scene, const aiNode* node, GameObject* parentObject,const char* file) //Load meshes, materials, textures, transforms
{
	GameObject* newGameObject;
	
	if (node->mParent == NULL)
	{
		std::string name;
		App->fileSystem->SplitFilePath(file, nullptr, &name);
		newGameObject = new GameObject(App->scene->rootObject, name.c_str());
	}
	else
	{
		newGameObject = new GameObject(parentObject, node->mName.C_Str());
	}

	node = ProcessTransform(node, newGameObject);

	if (node->mNumMeshes > 0)
	{
		ProcessMeshes(scene, node, newGameObject);
	}

	ProcessMaterial(scene, node, newGameObject, file);

	//parentObject->children.push_back(newGameObject);  //Add object
	App->scene->AddGameObject(newGameObject);
	
	
	for (int i = 0; i < node->mNumChildren; i++) //Process children
	{
		ProcessAiNode(scene,node->mChildren[i],newGameObject,file);
	}
}

const aiNode* Importer::SceneImporter::ProcessTransform(const aiNode* node, GameObject* newGameObject)
{
	aiVector3D position = { 0,0,0 };
	aiVector3D scale = { 0,0,0 };
	aiQuaternion rotation;

	aiVector3D _position = { 0,0,0 };
	aiVector3D _scale = { 0,0,0 };
	aiQuaternion _rotation = { 0,0,0,0 };

	while (strstr(node->mName.C_Str(), "_$AssimpFbx$_") != nullptr && node->mNumChildren == 1)
	{
		node->mTransformation.Decompose(_scale, _rotation, _position);

		position += _position;

		scale.x *= _scale.x;
		scale.y *= _scale.y;
		scale.z *= _scale.z;

		rotation = rotation * _rotation;
		node = node->mChildren[0];
		newGameObject->SetName(node->mName.C_Str());
	}

	node->mTransformation.Decompose(_scale, _rotation, _position);

	scale += _scale;
	rotation = rotation * _rotation;
	position += _position;

	float3 p = {position.x, position.y, position.z};
	float3 s = { scale.x, scale.y, scale.z };
	Quat r = { rotation.x,rotation.y,rotation.z,rotation.w };

	newGameObject->transform->SetLocalTransform(p, s, r);

	return node;
}

void Importer::SceneImporter::ProcessMeshes(const aiScene* scene, const aiNode* node, GameObject* newGameObject)
{
	std::vector<RMesh*> meshes;

	Importer::MeshImporter::LoadNodeMesh(scene,node,meshes);

	for (int i = 0; i < meshes.size(); i++)
	{
		newGameObject->AddComponent(new CMesh(newGameObject,"",meshes[i]));

		Importer::MeshImporter::Save(*meshes[i],node->mName.C_Str()); //Saving by name .mesh file
	}
	
}

void Importer::SceneImporter::ProcessMaterial(const aiScene* scene, const aiNode* node, GameObject* newGameObject, const char* file)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		RMaterial* material = new RMaterial();
		uint index = scene->mMeshes[node->mMeshes[i]]->mMaterialIndex;
		aiString path;

		std::string fileName, extension;

		aiColor4D color;
		if (scene->mMaterials[index]->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
		{
			material->SetColor(color.r,color.g,color.b,color.a);
		}

		RTexture* texture = nullptr;


		if (index >= 0)
		{
			if (scene->mMaterials[index]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				
				App->fileSystem->SplitFilePath(path.C_Str(), nullptr, &fileName, &extension);

				fileName += "." + extension;
				fileName = "Assets/Textures/" + fileName;

				LOG("Adding texture to %s", newGameObject->GetName());

				uint uid = App->resourceManager->ImportFile(fileName.c_str(), ResourceType::texture);

				texture = (RTexture*)App->resourceManager->RequestResource(uid);
				//SetTexture(Importer::TextureImp::Import(fileName.c_str()));

			}
			else
			{
				LOG("(ERROR) Failed loading node texture: %s in node %s",path.C_Str(),node->mName.C_Str());
			}
		}

		newGameObject->AddComponent(new CMaterial(newGameObject, fileName.c_str(), material,texture));

	}
}
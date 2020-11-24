#include "Application.h"
#include "GameObject.h"
#include "Resource.h"

#include "MFileSystem.h"
#include "MScene.h"

#include "CTransform.h"
#include "CMesh.h"
#include "CMaterial.h"

#include "IScene.h"
#include "IMesh.h"
#include "ITexture.h"

#include "Dependecies/Assimp/include/mesh.h"
#include "Dependecies/Assimp/include/cimport.h"
#include "Dependecies/Assimp/include/scene.h"
#include "Dependecies/Assimp/include/postprocess.h"

#include "Dependecies/MathGeoLib/src/MathGeoLib.h"
#include "Dependecies/mmgr/mmgr.h"

void Importer::SceneImporter::Import(const char* file) //Load buffer with .fbx scene
{
	char* buffer;
	uint byteSize = App->fileSystem->Load(file, &buffer);

	LOG("Began importing: %s", file);

	if (byteSize > 0)
	{
		const aiScene* scene = aiImportFileFromMemory(buffer, byteSize, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);
		const aiNode* rootNode = scene->mRootNode;

		ProcessAiNode(scene,rootNode,App->scene->rootObject,file); //Process node tree

		LOG("Finished importing: %s", file);
	}
	else
	{
		LOG("(ERROR) Could not load .fbx file");
	}
	
}

void Importer::SceneImporter::ProcessAiNode(const aiScene* scene, const aiNode* node, GameObject* parentObject,const char* file) //Load meshes, materials, textures, transforms
{
	GameObject* newGameObject;
	
	float3 position = { 0,0,0 };
	float3 scale = { 0,0,0 };
	Quat rotation = Quat::identity;

	while (strstr(node->mName.C_Str(), "_$AssimpFbx$_") != nullptr)
	{
		aiVector3D _position = { 0,0,0 };
		aiVector3D _scale = { 0,0,0 };
		aiQuaternion _rotation;

		node->mTransformation.Decompose(_scale, _rotation, _position);

		position.x += _position.x;
		position.y += _position.y;
		position.z += _position.z;

		scale.x += _scale.x;
		scale.y += _scale.y;
		scale.z += _scale.z;

		rotation.x += _rotation.x;
		rotation.y += _rotation.y;
		rotation.z += _rotation.z;
		rotation.w += _rotation.w;

		node = node->mChildren[0];
	}

	if (node->mParent == NULL)
	{
		std::string name;
		App->fileSystem->SplitFilePath(file, nullptr, &name);
		newGameObject = new GameObject(App->scene->rootObject, name.c_str());
	}
	else
	{
		aiString nodeName = node->mName;
		newGameObject = new GameObject(parentObject, nodeName.C_Str());
	}

	LoadTransform(node, newGameObject);

	if (node->mNumMeshes > 0)
	{
		LoadMeshes(scene, node, newGameObject);
	}

	LoadMaterial(scene, node, newGameObject, file);

	//Add object
	parentObject->children.push_back(newGameObject);
	App->scene->gameObjects.push_back(newGameObject);
	

	//Iterate children
	for (int i = 0; i < node->mNumChildren; i++)
	{
		ProcessAiNode(scene,node->mChildren[i],newGameObject,file);
	}
}

void Importer::SceneImporter::LoadTransform( const aiNode* node, GameObject* newGameObject)
{
	aiVector3D position;
	aiVector3D scale;
	aiQuaternion rotation;

	node->mTransformation.Decompose(scale, rotation, position);

	float3 _position = {position.x, position.y, position.z};
	float3 _scale = { scale.x, scale.y, scale.z };
	Quat _rotation = { rotation.x,rotation.y,rotation.z,rotation.w };

	newGameObject->transform->SetLocalTransform(_position, _scale, _rotation);
}

void Importer::SceneImporter::LoadMeshes(const aiScene* scene, const aiNode* node, GameObject* newGameObject)
{
	std::vector<RMesh*> meshes;

	Importer::MeshImporter::LoadNodeMesh(scene,node,meshes);

	for (int i = 0; i < meshes.size(); i++)
	{
		newGameObject->AddComponent(new CMesh(newGameObject,"",meshes[i]));
	}
	
}

void Importer::SceneImporter::LoadMaterial(const aiScene* scene, const aiNode* node, GameObject* newGameObject, const char* file)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		RMaterial* texture;
		uint index = scene->mMeshes[node->mMeshes[i]]->mMaterialIndex;
		aiString path;

		aiColor3D color;
		scene->mMaterials[index]->Get(AI_MATKEY_COLOR_DIFFUSE, color);

		

		if (index >= 0)
		{
			if (scene->mMaterials[index]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				std::string fileName, extension;
				App->fileSystem->SplitFilePath(path.C_Str(), nullptr, &fileName, &extension);

				fileName += "." + extension;
				fileName = "Assets/Textures/" + fileName;

				LOG("Adding texture to %s", newGameObject->GetName());

				texture = Importer::TextureImp::Import(fileName.c_str());

				newGameObject->AddComponent(new CMaterial(newGameObject, fileName.c_str(), texture));
			}
			else
			{
				LOG("(ERROR) Failed loading node texture: %s in node %s",path.C_Str(),node->mName.C_Str());
			}
		}

	}
}
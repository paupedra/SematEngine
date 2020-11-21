#include "Application.h"
#include "I_Scene.h"
#include "I_Mesh.h"
#include "ModuleFileSystem.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"

#include "ModuleSceneIntro.h"

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

		ProcessAiNode(scene,rootNode,App->scene_intro->rootObject); //Process node tree

		LOG("Finished importing: %s", file);
	}
	else
	{
		LOG("(ERROR) Could not load .fbx file");
	}
	
}

void Importer::SceneImporter::ProcessAiNode(const aiScene* scene, const aiNode* node, GameObject* parentObject) //Load meshes, materials, textures, transforms
{
	GameObject* newGameObject;

	if (node->mParent == NULL)
		newGameObject = new GameObject(App->scene_intro->rootObject, "Defaul");
	else
		newGameObject = new GameObject(parentObject, "Defaul");

	aiString nodeName = node->mName;
	newGameObject->SetName(nodeName.C_Str());

	//Load things
	LoadTransform(node, newGameObject);
	
	if (node->mNumMeshes > 0)
	{
		LoadMeshes(scene, node, newGameObject);
	}
	//_&AssimpFbx&_ dummy

	//Add object
	parentObject->children.push_back(newGameObject);

	App->scene_intro->gameObjects.push_back(newGameObject);

	//Iterate children
	for (int i = 0; i < node->mNumChildren; i++)
	{
		ProcessAiNode(scene,node->mChildren[i],newGameObject);
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

	newGameObject->transform->SetTransform(_position, _scale, _rotation);
}

void Importer::SceneImporter::LoadMeshes(const aiScene* scene, const aiNode* node, GameObject* newGameObject)
{
	std::vector<Mesh*> meshes;

	Importer::MeshImporter::LoadNodeMesh(scene,node,meshes);

	for (int i = 0; i < meshes.size(); i++)
	{
		newGameObject->AddComponent(new ComponentMesh(newGameObject,"",meshes[i]));
	}
	
}
#include "Application.h"
#include "I_Scene.h"
#include "ModuleFileSystem.h"
#include "GameObject.h"
#include "ComponentTransform.h"

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

	if (byteSize > 0)
	{
		const aiScene* scene = aiImportFileFromMemory(buffer, byteSize, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);
		const aiNode* rootNode = scene->mRootNode;

		ProcessAiNode(rootNode,nullptr); //Process node tree
	}
	else
	{
		LOG("(ERROR) Could not load .fbx file");
	}
}

void Importer::SceneImporter::ProcessAiNode(const aiNode* node, GameObject* parentObject) //Load meshes, materials, textures, transforms
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
	
	//_&AssimpFbx&_ dummy

	//Add object
	if (parentObject == nullptr)
		App->scene_intro->rootObject->children.push_back(newGameObject);
	else
		parentObject->children.push_back(newGameObject);

	App->scene_intro->gameObjects.push_back(newGameObject);

	//Iterate children
	for (int i = 0; i < node->mNumChildren; i++)
	{
		ProcessAiNode(node->mChildren[i],newGameObject);
	}
}

void Importer::SceneImporter::LoadTransform(const aiNode* node, GameObject* newGameObject)
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

void Importer::SceneImporter::LoadMeshes(const aiNode* node, GameObject* newGameObject)
{

}
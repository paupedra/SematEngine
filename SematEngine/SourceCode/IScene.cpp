#include "Application.h"
#include "GameObject.h"
#include "Resource.h"
#include "Config.h"
#include "Component.h"
#include "Resource.h"
#include "Random.h"

#include "MFileSystem.h"
#include "MScene.h"
#include "MResourceManager.h"

#include "CTransform.h"
#include "CMesh.h"
#include "CMaterial.h"

#include "IScene.h"
#include "IMesh.h"
#include "ITexture.h"
#include "IMaterial.h"

#include "RMesh.h"
#include "RMaterial.h"
#include "RScene.h"
#include "RModel.h"

#include "Dependecies/Assimp/include/mesh.h"
#include "Dependecies/Assimp/include/cimport.h"
#include "Dependecies/Assimp/include/scene.h"
#include "Dependecies/Assimp/include/postprocess.h"

#include "Dependecies/MathGeoLib/include/MathGeoLib.h"
#include "Dependecies/mmgr/mmgr.h"

void Importer::SceneImporter::ImportSceneResource(const char* buffer, RScene* resource,uint size)
{
	if (size > 0)
	{
		const aiScene* scene = aiImportFileFromMemory(buffer, size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);
		const aiNode* rootNode = scene->mRootNode;

		
		//save all the meshes and add them to RScene->meshes (list containing the id of the meshes, models will use this to get their id)
		Importer::MeshImporter::ImportAllMeshesInScene(scene,resource->meshes);
		Importer::MaterialImporter::ImportAllMaterialsInScene(scene, resource->materials);

		//add model
		ProcessAiNodeModel(scene, rootNode, resource,0); //Process node tree

		RScene* sceneResource = (RScene*)resource;
		sceneResource->GenerateCustomFile(); //return serialized json size

		LOG("Finished importing: %s",resource->resourceData.assetsFile.c_str());
	}
	else
	{
		LOG("(ERROR) Could not load .fbx file");
	}
}

void Importer::SceneImporter::ProcessAiNodeModel(const aiScene* scene, const aiNode* node, RScene* _scene,UID parentUID)
{
	RModel model(Random::GenerateUID());
	
	model.parentUID = parentUID;

	node = ProcessTransformModel(node, &model);

	if (node->mNumMeshes > 0)
	{
		ProcessMeshesModel(scene, node, &model,_scene);

		ProcessMaterialModel(scene, node, &model);
	}
	_scene->models.push_back(model);

	for (int i = 0; i < node->mNumChildren; i++) //Process children
	{
		ProcessAiNodeModel(scene, node->mChildren[i], _scene,model.uid);
	}
}

const aiNode* Importer::SceneImporter::ProcessTransformModel(const aiNode* node, RModel* model)
{
	aiVector3D position = { 0,0,0 };
	aiVector3D scale = { 0,0,0 };
	aiQuaternion rotation = { 0,0,0 };

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
		model->name = node->mName.C_Str();
	}

	node->mTransformation.Decompose(_scale, _rotation, _position);

	scale += _scale;
	rotation = rotation * _rotation;
	position += _position;

	float3 p = { position.x, position.y, position.z };
	float3 s = { scale.x, scale.y, scale.z };
	Quat r = { rotation.x,rotation.y,rotation.z,rotation.w };

	model->transform = float4x4::FromTRS(p, r, s);

	return node;
}

void Importer::SceneImporter::ProcessMeshesModel(const aiScene* scene, const aiNode* node, RModel* model,RScene* _scene)
{
	std::vector<uint> meshesUID;
	std::vector<RMesh> meshes;

	model->name = node->mName.C_Str();

	//only do first mesh for safety
	if(node->mNumMeshes > 0)
		model->mesheUID = node->mMeshes[0];

	//for (int i = 0; i < node->mNumMeshes; i++)
	//{
	//	meshesUID.push_back(_scene->meshes[node->mMeshes[i]]); //push the loaded uid of the mesh in RScene meshes previously saved and generated
	//}
}

void Importer::SceneImporter::ProcessMaterialModel(const aiScene* scene, const aiNode* node,RModel* model)
{
	model->materialUID = scene->mMeshes[node->mMeshes[0]]->mMaterialIndex; // must be adapted for various meshes
}

GameObject* Importer::SceneImporter::LoadSceneResource(ModelNode node)
{
	//Start creating game objects and requesting meshes through UID (loading it into memory in resources map), if the UID is already loaded it will return it
	GameObject* newGameObject;
	if (node.parent == nullptr)
	{
		newGameObject = new GameObject(App->scene->rootObject,node.model.name.c_str());
	}
	else
	{
		newGameObject = new GameObject( node.model.name.c_str()); //We'll set parent later
	}

	//transform
	newGameObject->transform->SetLocalTransform(node.model.transform);

	//mesh
	if (node.model.mesheUID != 0) //if there is a mesh
	{
		CMesh* cMesh = new CMesh(newGameObject);

		if (node.model.mesheUID != 0)
		{
			App->resourceManager->LoadModelResource(node.model.mesheUID, ResourceType::mesh);
			RMesh* loadedMesh = (RMesh*)App->resourceManager->RequestResource(node.model.mesheUID);

			newGameObject->AddComponent(cMesh);
			cMesh->SetMesh(loadedMesh);
		
		//texture
			if (node.model.materialUID != 0)
			{
				CMaterial* cmaterial = new CMaterial(newGameObject);
				newGameObject->AddComponent(cmaterial);
				//uint textureUID = App->resourceManager->LoadResource(node.model.materialUID); //got the material, now we need to extract the color and the texture uid

				cmaterial->SetMaterial(App->resourceManager->LoadMaterial(node.model.materialUID));

				//RTexture* texture = (RTexture*)App->resourceManager->RequestResource(textureUID);
				//cmaterial->SetTexture(texture);
				//cmaterial->SetMaterial(App->resourceManager->LoadMaterial(node.model.materialUID));

			}
		}
	}

	App->scene->AddGameObject(newGameObject);
	LOG("Adding game Object: %s", newGameObject->GetName());

	for (std::vector<ModelNode*>::iterator child = node.children.begin(); child != node.children.end(); child++)
	{
		GameObject* object = LoadSceneResource(*(*child));
		newGameObject->AddChild(object);
	}
	
	return newGameObject;
}

uint Importer::SceneImporter::ImportMaterial(aiMaterial* material, RMaterial* resource)
{

	return 0;
}
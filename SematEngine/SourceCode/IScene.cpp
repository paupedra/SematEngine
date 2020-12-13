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
			//LOG("Mesh has %d whatever", scene->mMeshes[i]->mNumVertices);
		}
		//LOG("There are %d meshes in this .fbx file" , i);

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

		//Importer::MeshImporter::Save(*meshes[i],node->mName.C_Str()); //Saving by name .mesh file
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

void Importer::SceneImporter::ImportSceneResource(const char* buffer, RScene* resource,uint size)
{

	if (size > 0)
	{
		const aiScene* scene = aiImportFileFromMemory(buffer, size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);
		const aiNode* rootNode = scene->mRootNode;

		
		//save all the meshes and add them to RScene->meshes (list containing the id of the meshes, models will use this to get their id)
		Importer::MeshImporter::LoadAllMeshesInScene(scene,resource->meshes);

		//add model
		ProcessAiNodeModel(scene, rootNode, resource,0); //Process node tree

		RScene* sceneResource = (RScene*)resource;
		sceneResource->GenerateCustomFile(); //return serialized json size

		LOG("Finished importing: %s");
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
	UID UID = 0;

	RMaterial* material = new RMaterial();
	uint index = scene->mMeshes[node->mMeshes[0]]->mMaterialIndex;
	aiString path;

	std::string fileName, extension;

	aiColor4D color;
	if (scene->mMaterials[index]->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
	{
		material->SetColor(color.r, color.g, color.b, color.a);
	}

	RTexture* texture = nullptr;

	if (index >= 0)
	{
		if (scene->mMaterials[index]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
		{
			App->fileSystem->SplitFilePath(path.C_Str(), nullptr, &fileName, &extension);

			fileName += "." + extension;
			fileName = "Assets/Textures/" + fileName;

			

		}
		else
		{
			LOG("(ERROR) Failed loading node texture: %s in node %s", path.C_Str(), node->mName.C_Str());
		}
	}
	
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
		}
		//texture
		//if()

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
#include "Random.h"
#include "Resource.h"

#include "IAnimation.h"

#include "RAnimation.h"

#include "Dependecies/Assimp/include/cimport.h"
#include "Dependecies/Assimp/include/scene.h"
#include "Dependecies/Assimp/include/postprocess.h"

//#include "Dependecies/mmgr/mmgr.h"

uint Importer::AnimationImporter::Import(const aiAnimation* animation)
{
	RAnimation newAnimation(Random::GenerateUID());

	newAnimation.name = animation->mName.C_Str();
	newAnimation.duration = animation->mDuration;
	newAnimation.speed = animation->mTicksPerSecond;

	for (int i = 0; i < animation->mNumChannels; i++)
	{
		Bone newBone;
		newBone.name = animation->mChannels[i]->mNodeName.C_Str();

		int p;
		for ( p = 0 ; p < animation->mChannels[i]->mNumPositionKeys; p++) //process position keys
		{
			VectorKey newKey;
			aiVector3D position = animation->mChannels[i]->mPositionKeys[p].mValue;
			newKey.value = float3(position.x, position.y, position.z);
			newKey.time = animation->mChannels[i]->mPositionKeys[p].mTime;
			newBone.positionKeys.push_back(newKey);
		}

		for (p = 0; p < animation->mChannels[i]->mNumScalingKeys; p++) 	//process scale keys
		{
			VectorKey newKey;
			aiVector3D scale = animation->mChannels[i]->mScalingKeys[p].mValue;
			newKey.value = float3(scale.x, scale.y, scale.z);
			newKey.time = animation->mChannels[i]->mScalingKeys[p].mTime;
			newBone.scaleKeys.push_back(newKey);
		}
		
		for (p = 0; p < animation->mChannels[i]->mNumRotationKeys; p++)	//process quaternion keys
		{
			QuaternionKey newKey;
			aiQuaternion rotation = animation->mChannels[i]->mRotationKeys[p].mValue;
			newKey.value = Quat(rotation.x, rotation.y, rotation.z, rotation.w);
			newKey.time = animation->mChannels[i]->mScalingKeys[p].mTime;
			newBone.quaternionKeys.push_back(newKey);
		}

		newAnimation.bones.push_back(newBone);
	}
	//save in cff

	LOG("Finished importing animation: %s", newAnimation.name.c_str());

	return newAnimation.GetUID();
}

void Importer::AnimationImporter::ImportAllAnimationsInScene(const aiScene* scene, std::vector<uint>& animations)
{
	if (!scene->HasAnimations())
		return;
	for (int i = 0; i < scene->mNumAnimations; i++)
	{
		animations.push_back(Import(scene->mAnimations[i]));
	}
}

uint64 Importer::AnimationImporter::Save(const RAnimation animation, const char* name)
{

	return 0;
}

void Importer::AnimationImporter::Load(const char* fileBuffer, RAnimation* mesh)
{

}
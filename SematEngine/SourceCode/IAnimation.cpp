#include "Application.h"
#include "Globals.h"
#include "Random.h"
#include "Resource.h"
#include "Config.h"

#include "MFileSystem.h"
#include "MResourceManager.h"

#include "RAnimation.h"
#include "RAnimationCollection.h"

#include "IAnimation.h"

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
		std::string boneName = animation->mChannels[i]->mNodeName.C_Str();

		int p;
		for ( p = 0 ; p < animation->mChannels[i]->mNumPositionKeys; p++) //process position keys
		{
			aiVector3D position = animation->mChannels[i]->mPositionKeys[p].mValue;
			newBone.positionKeys.emplace(animation->mChannels[i]->mPositionKeys[p].mTime, float3(position.x, position.y, position.z));
		}
		for (p = 0; p < animation->mChannels[i]->mNumScalingKeys; p++) 	//process scale keys
		{
			aiVector3D scale = animation->mChannels[i]->mScalingKeys[p].mValue;
			newBone.scaleKeys.emplace(animation->mChannels[i]->mScalingKeys[p].mTime, float3(scale.x, scale.y, scale.z));
		}
		for (p = 0; p < animation->mChannels[i]->mNumRotationKeys; p++)	//process quaternion keys
		{
			aiQuaternion rotation = animation->mChannels[i]->mRotationKeys[p].mValue;
			newBone.quaternionKeys.emplace(animation->mChannels[i]->mRotationKeys[p].mTime, Quat(rotation.x, rotation.y, rotation.z, rotation.w));
		}

		newAnimation.bones.emplace(boneName,newBone);
	}
	//save in cff
	newAnimation.GenerateCustomFile();

	//App->resourceManager->AddResourceToLibrary((Resource*)&newAnimation);

	LOG("Finished importing animation: %s", newAnimation.name.c_str());

	return newAnimation.GetUID();
}

void Importer::AnimationImporter::ImportAllAnimationsInScene(const aiScene* scene, RAnimationCollection* animations)
{
	if (!scene->HasAnimations())
		return;

	for (int i = 0; i < scene->mNumAnimations; i++)
	{
		animations->animations.push_back(Import(scene->mAnimations[i]));
	}

	animations->GenerateCustomFile();
}

uint64 Importer::AnimationImporter::Save(RAnimation* animation, const char* name)
{
	//save basic animation data
	uint size = sizeof(uint) + sizeof(char) * animation->name.size() + sizeof(uint) + sizeof(double) + sizeof(double);

	//calc size iterating channels
	std::map<std::string, Bone>::const_iterator i;
	for (i = animation->bones.begin(); i != animation->bones.end(); i++)
	{
		size += sizeof(uint);
		size += sizeof(char) * (*i).first.size();
		size += sizeof(uint) * 3; //sizes of maps
		size += (sizeof(double) + sizeof(float)*3)	 * (*i).second.positionKeys.size();
		size += (sizeof(double) + sizeof(float) * 3) * (*i).second.scaleKeys.size();
		size += (sizeof(double) + sizeof(float) * 4) * (*i).second.quaternionKeys.size();
	}

	char* buffer = nullptr;
	buffer = new char[size];
	char* cursor = buffer;

	uint nameSize[1] = {animation->name.size()}; //save  name
	memcpy(cursor, nameSize, sizeof(uint));
	cursor += sizeof(uint);

	memcpy(cursor, animation->name.c_str(), sizeof(char) * animation->name.size());
	cursor += sizeof(char) * animation->name.size();

	uint bonesSize[1] = { animation->bones.size() };//size of bones
	uint bytes = sizeof(bonesSize); 
	memcpy(cursor, bonesSize, bytes);
	cursor += bytes;

	double times[2] = { animation->duration, animation->speed }; //duration + speed
	bytes = sizeof(times); 
	memcpy(cursor, times, bytes);
	cursor += bytes;

	for (i = animation->bones.begin(); i != animation->bones.end(); i++)
	{
		uint nameSize[1] = { (*i).first.size() };
		memcpy(cursor, nameSize, sizeof(uint));
		cursor += sizeof(uint);

		memcpy(cursor, (*i).first.c_str(), sizeof(char) * nameSize[0]);
		cursor += sizeof(char) * nameSize[0];

		SaveBones(&cursor, (*i).second );
	}
	
	LOG("Saved animation in file: %s", name);

	App->fileSystem->Save(name, buffer, size); 

	RELEASE_ARRAY(buffer);

	return 0;
}

void Importer::AnimationImporter::SaveBones(char** cursor, Bone bone)
{
	
	uint boneSizes[3] = { bone.positionKeys.size() , bone.scaleKeys.size(), bone.quaternionKeys.size() };

	memcpy(*cursor, boneSizes, sizeof(boneSizes));
	*cursor += sizeof(boneSizes);

	//position loop
	std::map<double, float3>::const_iterator it;
	for (it = bone.positionKeys.begin(); it != bone.positionKeys.end(); it++)
	{
		memcpy(*cursor, &it->second, sizeof(float)*3);
		*cursor += sizeof(float) * 3;

		memcpy(*cursor, &it->first, sizeof(double));
		*cursor += sizeof(double);
	}
	//scale loop
	for (it = bone.scaleKeys.begin(); it != bone.scaleKeys.end(); it++)
	{
		memcpy(*cursor, &it->second, sizeof(float) * 3);
		*cursor += sizeof(float) * 3;

		memcpy(*cursor, &it->first, sizeof(double));
		*cursor += sizeof(double);
	}
	//quat loop
	std::map<double, Quat>::const_iterator it2;
	for (it2 = bone.quaternionKeys.begin(); it2 != bone.quaternionKeys.end(); it2++)
	{
		memcpy(*cursor, &it2->second, sizeof(float)*4);
		*cursor += sizeof(float) * 4;

		memcpy(*cursor, &it2->first, sizeof(double));
		*cursor += sizeof(double);
	}
}

void Importer::AnimationImporter::Load(const char* fileBuffer, RAnimation* animation)
{
	//read basic data
	const char* cursor = fileBuffer;

	uint animNameSize[1];
	memcpy(animNameSize, cursor, sizeof(uint));
	cursor += sizeof(uint);

	animation->name.resize(animNameSize[0]);
	memcpy(&animation->name[0], cursor, sizeof(char) * animNameSize[0]);
	cursor += sizeof(char) * animNameSize[0];

	uint bonesSize[1];
	uint bytes = sizeof(bonesSize);
	memcpy(bonesSize, cursor, bytes);
	cursor += bytes;

	double time[2];
	bytes = sizeof(time);
	memcpy(time, cursor, bytes);
	cursor += bytes;

	animation->duration = time[0];
	animation->speed = time[1];

	for (int i = 0; i < bonesSize[0]; i++)
	{
		uint nameSize[1];
		memcpy(nameSize, cursor, sizeof(uint));
		cursor += sizeof(uint);

		std::string boneName;
		boneName.resize(nameSize[0]);
		memcpy(&boneName[0], cursor, sizeof(char) * nameSize[0]);
		cursor += sizeof(char) * nameSize[0];

		uint boneSizes[3];

		bytes = sizeof(boneSizes);
		memcpy(boneSizes, cursor, bytes);
		cursor += bytes;

		Bone newBone;
		int p;
		for (p = 0; p < boneSizes[0]; p++)
		{
			std::map<double, float3> pos = LoadVector3Key(&cursor);
			newBone.positionKeys.emplace(pos.begin()->first, pos.begin()->second);
		}
		for (p = 0; p < boneSizes[1]; p++)
		{
			std::map<double, float3> scale = LoadVector3Key(&cursor);
			newBone.scaleKeys.emplace(scale.begin()->first, scale.begin()->second);
		}
		for (p = 0; p < boneSizes[2]; p++)
		{
			std::map<double, Quat> rot = LoadQuatKey(&cursor);
			newBone.quaternionKeys.emplace(rot.begin()->first, rot.begin()->second);
		}

		animation->bones.emplace(boneName, newBone);
	}
}

std::vector<RAnimation*> Importer::AnimationImporter::LoadAnimationCollection(uint collection)
{
	std::vector<RAnimation*> ret;

	//Load animation collection file
	std::string path = ANIMATIONS_PATH;
	path += std::to_string(collection);
	path += ANIMATION_EXTENSION;

	char* buffer = nullptr;
	App->fileSystem->Load(path.c_str(), &buffer);

	JsonNode node(buffer);
	JsonArray animationsJson = node.GetArray("Animations");

	std::vector<uint> animationsUID;

	for (int i = 0; i < animationsJson.size; i++)
	{
		uint uid = animationsJson.GetNumber(i, 0);
		animationsUID.push_back(uid);

		//now load the animations in RAnimation vector (ret)

		//they should be added to resource map

		RAnimation* res = (RAnimation*)App->resourceManager->RequestResource(uid);
		if (res != nullptr)
		{
			ret.push_back(res);
		}
		else
		{
			//load it
			std::string path = ANIMATIONS_PATH;
			path += std::to_string(uid);
			path += ANIMATION_EXTENSION;

			char* secondBuffer = nullptr;

			App->fileSystem->Load(path.c_str(),&secondBuffer);

			res = new RAnimation(uid);

			Load(secondBuffer, res);

			ret.push_back(res);

			RELEASE_ARRAY(secondBuffer);
		}
	}

	RELEASE_ARRAY(buffer);

	return ret;
}

std::map<double,float3> Importer::AnimationImporter::LoadVector3Key(const char** cursor)
{
	std::map<double, float3> ret;
	float vector[3];

	uint bytes = sizeof(float) * 3;
	memcpy(vector, *cursor, bytes);
	*cursor += bytes;

	double time[1];

	bytes = sizeof(double);
	memcpy(time, *cursor, bytes);
	*cursor += bytes;

	float3 vector3 = float3(vector[0], vector[1], vector[2]);
	ret.emplace(time[0], vector3);

	return ret;
}

std::map<double, Quat> Importer::AnimationImporter::LoadQuatKey(const char** cursor)
{
	std::map<double, Quat> ret;
	float vector[4];

	uint bytes = sizeof(float) * 4;
	memcpy(vector, *cursor, bytes);
	*cursor += bytes;

	double time[1];

	bytes = sizeof(double);
	memcpy(time, *cursor, bytes);
	*cursor += bytes;

	Quat quat = Quat(vector[0], vector[1], vector[2],vector[3]);
	ret.emplace(time[0], quat);

	return ret;
}
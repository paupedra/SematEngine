#ifndef __RESOURCEMODEL_H__
#define __RESOURCEMODEL_H__

#include "Globals.h"
#include <vector>
#include <string>
#include "Dependecies/MathGeoLib/include/Math/float4x4.h"

class JsonArray;

struct RModel //This resource holds it's meshes/materials uids
{
	RModel(){}
	RModel(uint UID)
	{
		uid = UID;
	}

	void ReadJsonNode(JsonNode node)
	{
		name = node.GetString("Name");
		uid = node.GetNumber("UID");
		parentUID = node.GetNumber("Parent UID");
		meshUID = node.GetNumber("Mesh ID");
		materialUID = node.GetNumber("Material ID");

		//transform
		JsonArray transformJson = node.GetArray("Transform");
		for (int i = 0; i < 16; i++)
		{
			transform.ptr()[i] = transformJson.GetNumber(i,0);
		}
	}

	float4x4 transform = float4x4::identity;
	std::string name;

	UID uid = 0; //The resource UID
	UID parentUID = 0;

	UID meshUID = -1; //Position in the vector of meshes uid of the RScene (much like assimp does) -1 if no mesh
	UID materialUID = 0;
};

struct ModelNode //Used to stre the info of a .scene file in a tree to be interpreted later
{
	ModelNode(uint UID)
	{
		model = RModel(UID);
	}

	RModel model;

	ModelNode* parent = nullptr;

	std::vector<ModelNode*> children;

};

#endif //__RESOURCEMODEL_H__
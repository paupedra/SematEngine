#ifndef __RESOURCEMODEL_H__
#define __RESOURCEMODEL_H__

#include "Globals.h"
#include <vector>
#include <string>
#include "Dependecies/MathGeoLib/include/Math/float4x4.h"

struct RModel //This resource holds it's meshes/materials uids
{
	RModel(){}
	RModel(uint UID)
	{
		uid = UID;
	}

	float4x4 transform = float4x4::identity;
	std::string name;

	UID uid = 0; //The resource UID
	UID parentUID = 0;

	uint mesheUID = 0; //Position in the vector of meshes uid of the RScene (much like assimp does)
	UID materialUID = 0;
};
#endif //__RESOURCEMODEL_H__
#ifndef __RESOURCEANIMATION_H__
#define __RESOURCEANIMATION_H__

class Resource;
#include <vector>
#include "Dependecies/MathGeoLib/include/Math/float3.h"
#include "Dependecies/MathGeoLib/include/Math/Quat.h"

struct VectorKey
{
	double time = 0;
	float3 value = float3::zero;
};

struct QuaternionKey
{
	double time = 0;
	Quat value = Quat::identity;
};

struct Bone
{
	std::string name;
	std::vector<VectorKey> positionKeys;
	std::vector<VectorKey> scaleKeys;
	std::vector<QuaternionKey> quaternionKeys;
};

class RAnimation : public Resource
{
public:

	RAnimation();
	RAnimation(uint UID);
	~RAnimation();

	void CleanUp() override;

	UID GenerateCustomFile(UID textureUID);

public:
	std::string name;
	double duration = 0;	//Ticks
	double speed = 1;		//Ticks per second
	std::vector<Bone> bones;
};
#endif //__RESOURCEANIMATION_H__
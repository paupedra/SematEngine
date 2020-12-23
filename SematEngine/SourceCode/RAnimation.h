#ifndef __RESOURCEANIMATION_H__
#define __RESOURCEANIMATION_H__

class Resource;
#include <vector>
#include <map>

#include "Dependecies/MathGeoLib/include/Math/float3.h"
#include "Dependecies/MathGeoLib/include/Math/Quat.h"


struct Bone
{
	std::map<double, float3> positionKeys; //double is time
	std::map<double, float3> scaleKeys;
	std::map<double, Quat> quaternionKeys;
};

class RAnimation : public Resource
{
public:

	RAnimation();
	RAnimation(uint UID);
	~RAnimation();

	void CleanUp() override;

	UID GenerateCustomFile();

public:
	std::map<std::string,Bone> bones;
	std::string name;
	double duration = 0;		//Ticks
	double speed = 1;			//Ticks per second
};
#endif //__RESOURCEANIMATION_H__
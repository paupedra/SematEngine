#ifndef __RESOURCEANIMATION_H__
#define __RESOURCEANIMATION_H__

class Resource;
#include <vector>
#include <map>

#include "Dependecies/MathGeoLib/include/Math/float3.h"
#include "Dependecies/MathGeoLib/include/Math/Quat.h"

class RAnimation;

struct AnimationChop
{
	AnimationChop(RAnimation* _owner , float _startKey, float _endKey, float _speed)
	{
		_startKey < 0 ? startKey = 0 : startKey = _startKey;
		endKey = _endKey;

		_speed <= 0 ? speed = 1 : speed = _speed;
	}

	void SetStartKey(float _startKey);
	void SetEndKey(float _endKey);
	void SetSpeed(float _speed);

	float startKey = 0;
	float endKey = 0;
	float speed = 1;
	RAnimation* owner= nullptr;
};

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
	std::vector<AnimationChop> chops;	//Contais chops that this animation is divided in
};
#endif //__RESOURCEANIMATION_H__
#ifndef __RESOURCEANIMATIONCONTROLLER_H__
#define __RESOURCEANIMATIONCONTROLLER_H__

#include <vector>

class Resource;
typedef unsigned int UID;
typedef unsigned int uint;

class RAnimationCollection : public Resource //A class that holds multiple animations
{
public:

	RAnimationCollection();
	RAnimationCollection(uint UID);
	~RAnimationCollection();

	void AddAnimation(UID UID);

	void CleanUp() override;

	UID GenerateCustomFile();

public:
	std::vector<UID> animations;
};

#endif //__RESOURCEANIMATIONCONTROLLER_H__
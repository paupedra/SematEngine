#ifndef __RESOURCESCENE_H__
#define __RESOURCESCENE_H__

#include "Globals.h"
#include <vector>
#include "RModel.h"
class Resource;

class RScene : public Resource
{
public:

	RScene();
	RScene(uint UID);
	~RScene();

	void CleanUp();

private:
	std::vector<RModel> models;
};
#endif //__RESOURCESCENE_H__
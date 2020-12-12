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

	UID GenerateCustomFile();

public:
	std::vector<RModel> models;
	std::vector<UID> meshes; //uids of the fbx meshes, RModels will reference this list
};
#endif //__RESOURCESCENE_H__
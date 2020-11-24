#ifndef __ModuleResourceManager_H__
#define __ModuleResourceManager_H__

#include "Module.h"
#include "Dependecies/SDL/include/SDL.h"

class Application;

class MResourceManager : public Module
{
public:

	MResourceManager(bool start_enabled = true);

	// Destructor
	virtual ~MResourceManager();

	bool Init();
	bool CleanUp();



public:

};

#endif // __ModuleResourceManager_H__
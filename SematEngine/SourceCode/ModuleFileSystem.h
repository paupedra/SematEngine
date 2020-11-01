#ifndef __MODULEFILESYSTEM_H__
#define __MODULEFILESYSTEM_H__

#include "Module.h"
#include <vector>
#include <string>

struct SDL_RWops;
int close_sdl_rwops(SDL_RWops* rw);

struct aiFileIO;

//struct BASS_FILEPROCS;
class Config;
struct PathNode;

class M_FileSystem : public Module
{
public:

	M_FileSystem(bool start_enabled = true);

	// Destructor
	~M_FileSystem();

	// Called before render is available
	bool Init() override;

	// Called before quitting
	bool CleanUp() override;

	void CreateLibraryDirectories();

	// Utility functions
	bool AddPath(const char* path_or_zip);
	bool Exists(const char* file) const;
};

#endif // __MODULEFILESYSTEM_H__

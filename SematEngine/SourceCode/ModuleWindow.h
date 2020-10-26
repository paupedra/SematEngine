#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "Dependecies/SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	int GetWidth() const;
	int GetHeight() const;

	void SetWidth(int width);
	void SetHeight(int height);

	void SetTitle(const char* title);

public:
	SDL_Window* window;
	SDL_Surface* screen_surface;

	//int width;
	//int height;
};

#endif // __ModuleWindow_H__
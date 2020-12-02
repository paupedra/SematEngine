#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"


class Application;
struct SDL_Window;
struct SDL_Surface;

class MWindow : public Module
{
public:

	MWindow(bool start_enabled = true);

	// Destructor
	virtual ~MWindow();

	bool Init();
	bool CleanUp();

	int GetWidth() const;
	int GetHeight() const;

	void SetWidth(int width);
	void SetHeight(int height);

	void SetTitle(const char* title);

public:
	SDL_Window* window;
	SDL_Surface* screenSurface;

	//int width;
	//int height;
};

#endif // __ModuleWindow_H__
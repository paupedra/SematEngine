#include "Globals.h"
#include "Application.h"

#include "MWindow.h"
#include "MRenderer3D.h"

#include "Dependecies/mmgr/mmgr.h"

MWindow::MWindow(bool start_enabled) : Module(start_enabled)
{
	window = NULL;
	screenSurface = NULL;
}

// Destructor
MWindow::~MWindow()
{
}

// Called before render is available
bool MWindow::Init()
{
	LOG("(INIT) Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("(ERROR) SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("(ERROR) Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screenSurface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool MWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

int MWindow::GetWidth() const
{
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	return w;
}

int MWindow::GetHeight() const
{
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	return h;
}

void MWindow::SetWidth(int width)
{
	int w = width;
	int h = GetHeight();
	SDL_SetWindowSize(window, w, h);
	App->renderer3D->OnResize(w, h);
}

void MWindow::SetHeight(int height)
{
	int w = GetWidth();
	int h = height;
	SDL_SetWindowSize(window, w, h);
	App->renderer3D->OnResize(w, h);
}

void MWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}
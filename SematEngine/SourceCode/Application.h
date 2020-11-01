#pragma once

#include <vector>
#include <string>
#include "Globals.h"
#include "Timer.h"
#include "Module.h"

class ModuleWindow;
class ModuleInput;
class ModuleSceneIntro;
class ModuleRenderer3D;
class ModuleCamera3D;
class Editor;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	Editor* editor;

	bool debug;
	bool renderPrimitives;
	bool vsync;

private:

	Timer	frame_timer;
	Timer	seconds_timer;
	uint	frame_cap_ms;
	float	dt;
	float	frame_cap;
	int		frame_count;

	bool wantToExit;

	std::vector<Module*> modules;
	std::string title;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void SetTitle(const char* title);
	void SetFrameCap(int cap);
	void SwitchVsync();

	void ExitApp();
	void OpenLink(const char* link);
	std::string ReadTxt(const char* path);
	 
	const char* GetTitle() const;
	float GetFrameCap() const;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};
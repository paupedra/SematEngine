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
class FileSystem;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	Editor* editor;
	FileSystem* fileSystem;

	bool debug;
	bool vsync;
	bool wantToExit;
	bool wantToSave;

private:

	Timer	frameTimer;
	Timer	secondsTimer;
	uint	frameCapMs;
	float	dt;
	float	frameCap;
	int		frameCount;

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
	void FrameCalculations();
	void Save();
};
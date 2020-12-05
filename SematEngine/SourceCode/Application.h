#pragma once

#include <vector>
#include <string>
#include "Globals.h"

class Timer;
class Module;
class MWindow;
class MInput;
class MScene;
class MRenderer3D;
class MCamera3D;
class MEditor;
class MFileSystem;
class MResourceManager;

class Application
{
public:
	MWindow* window;
	MInput* input;
	MScene* scene;
	MRenderer3D* renderer3D;
	MCamera3D* camera;
	MEditor* editor;
	MFileSystem* fileSystem;
	MResourceManager* resourceManager;

	bool debug;
	bool vsync;

private:

	bool wantToExit;
	bool wantToSave;
	bool cleaningUp = false;

	Timer*	frameTimer;
	Timer*	secondsTimer;
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

	void WantToSave();
	void ExitApp();
	void OpenLink(const char* link);
	std::string ReadTxt(const char* path);
	 
	const char* GetTitle() const;
	float GetFrameCap() const;

	bool IsCleaningUp() const;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
	void FrameCalculations();
	void Save();
};
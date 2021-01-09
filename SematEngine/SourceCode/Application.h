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

	Application();
	~Application();

	bool Init();
	updateStatus Update();
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

	void Play();
	void Pause();
	void Stop();

	inline void SetTimeMultiplier(float _timeMultiplier) { timeMultiplier = _timeMultiplier; };

	inline bool IsInPlayMode()const { return play; };
	inline bool IsExiting()const { return wantToExit; };

	inline float GetDt()const		{ return dt; }
	inline float GetPlayDt()const	{ return playDt; };
	inline float GetPlayTime()const { return playTime; };
	inline float GetTimeMultiplier()const { return timeMultiplier; };

private:
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
	void FrameCalculations();
	void Save();

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
	bool wantToExit = false;
	bool wantToSave = false;
	bool cleaningUp = false;

	bool  play = false;
	bool  paused = false;
	float playTime = 0;
	float timeMultiplier = 1;

	Timer*	frameTimer = nullptr;
	Timer*	secondsTimer = nullptr;
	uint	frameCapMs=0;

	float	playDt = 0;	//This dt effects objects in Play mode
	float	dt = 0;		//independent App dt
	float	frameCap = 0;
	int		frameCount = 0;

	std::vector<Module*> modules;
	std::string title;

};
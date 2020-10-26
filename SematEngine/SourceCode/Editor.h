#pragma once
#include "Module.h"
#include "Globals.h"
#include <vector>

class Window;
class w_Console;
class w_Configuration;
class w_About;
class w_Hierarchy;
class w_Inspector;

class Editor : public Module
{
public:
	Editor(bool start_enabled = true);
	~Editor();

	bool Start();
	update_status PreUpdate(float dt) override;
	bool CleanUp();

	void Draw();

	void AddWindow(Window* window);

	void UpdateConfigFPS(int fps);
	void UpdateConfigMS(int ms);
	void AddLog(char* text);

private:

	std::vector<Window*> windows;

public:

	w_Console* console = nullptr;
	w_Configuration* configuration = nullptr;
	w_About* about = nullptr;
	w_Hierarchy* hierarchy = nullptr;
	w_Inspector* inspector = nullptr;
};
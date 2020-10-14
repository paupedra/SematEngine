#pragma once
#include "Module.h"
#include "Globals.h"
#include <vector>

class Window;
class w_Console;
class w_Configuration;

class Editor : public Module
{
public:
	Editor(bool start_enabled = true);
	~Editor();

	bool Start();
	update_status PreUpdate(float dt) override;
	bool CleanUp();

	void Draw();

	void AddWindow(Window* _window);

private:

	std::vector<Window*> windows;

public:

	w_Console* console;
	w_Configuration* configuration;

};
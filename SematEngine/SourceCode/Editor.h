#pragma once
#include "Module.h"
#include "Globals.h"

class Editor : public Module
{
public:
	Editor(bool start_enabled = true);
	~Editor();

	bool Start();
	update_status PreUpdate(float dt) override;
	bool CleanUp();

	void Draw();

private:

};
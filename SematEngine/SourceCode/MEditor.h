#pragma once
#include "Module.h"
#include "Globals.h"
#include <vector>

class Window;
class WConsole;
class WConfiguration;
class WAbout;
class WHierarchy;
class WInspector;
class WAssets;
class WPlay;
class WResources;
class WGame;
struct HardwareInfo;
class JsonNode;

class MEditor : public Module
{
public:
	MEditor(bool start_enabled = true);
	~MEditor();

	bool Start();
	updateStatus PreUpdate(float dt) override;
	updateStatus PostUpdate(float dt) override;
	bool CleanUp();
	bool Save(JsonNode* config) override;

	void Draw();
	void DrawMainMenuBar();

	void AddWindow(Window* window);
	void AddLog(char* text);
	void AddInput(const char* input);

	void UpdateConfigFPS(int fps);
	void UpdateConfigMS(int ms);

	void SetUpDocking();
	void SetHardwareInfo(HardwareInfo* hardware);

	bool IsMouseHovering();

private:

	std::vector<Window*> windows;

public:
	bool dockActive=true;
	WConsole* console = nullptr;
	WConfiguration* configuration = nullptr;
	WAbout* about = nullptr;
	WHierarchy* hierarchy = nullptr;
	WInspector* inspector = nullptr;
	WAssets* assets = nullptr;
	WPlay* play = nullptr;
	WResources* resources = nullptr;
	WGame* game = nullptr;
	bool mouseHovered = false;
};
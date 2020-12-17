#include "Application.h"
#include "Config.h"
#include "Module.h"
#include "Timer.h"

#include "MCamera3D.h"
#include "MInput.h"
#include "MRenderer3D.h"
#include "MScene.h"
#include "MWindow.h"
#include "MEditor.h"
#include "MFileSystem.h"
#include "MResourceManager.h"

#include "Dependecies/Brofiler/Brofiler.h"
#include "Dependecies/mmgr/mmgr.h"

Application::Application() : debug(false), dt(0.16f)
{
	window = new MWindow();
	input = new MInput();
	scene = new MScene();
	renderer3D = new MRenderer3D();
	camera = new MCamera3D();
	editor = new MEditor();
	fileSystem = new MFileSystem();
	resourceManager = new MResourceManager();

	// They will CleanUp() in reverse order
	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(editor);
	AddModule(fileSystem);
	
	// Scenes
	AddModule(scene);
	AddModule(resourceManager);//Resources must be kept to be dereferenced and released

	// Renderer last!
	AddModule(renderer3D);

	title = "Semat Engine";
	wantToExit = false;
	wantToSave = false;
	frameCap = 120;
}

Application::~Application()
{
	std::vector<Module*>::iterator item = modules.end();

	for(;item != modules.begin(); --item) {
		delete (*item);
	}
}

bool Application::Init()
{
	bool ret = true;
	App = this;

	// Call Init() in all modules
	std::vector<Module*>::iterator item = modules.begin();

	for (; item != modules.end() && ret == true;++item) {
		ret = (*item)->Init();
	}

	SetTitle("Semat Engine");

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = modules.begin();

	for (; item != modules.end() && ret == true; ++item) {
		ret = (*item)->Start();
	}
	
	frameTimer = new Timer();
	secondsTimer = new Timer();
	frameCap = 120;

	vsync = VSYNC;

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)frameTimer->Read() / 1000.0f;
	frameTimer->Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	FrameCalculations();
	
	if(wantToSave)
		Save();
}

void Application::FrameCalculations()
{
	frameCapMs = 1000 / frameCap;
	uint current_frame_ms = frameTimer->Read();

	if (!vsync && current_frame_ms < frameCapMs)
	{
		SDL_Delay(frameCapMs - current_frame_ms);
	}

	App->editor->UpdateConfigMS(frameTimer->Read());

	if (secondsTimer->Read() >= 1000)
	{
		secondsTimer->Start();
		App->editor->UpdateConfigFPS(frameCount);

		frameCount = 0;
	}

	frameCount++;
}

void Application::Save()
{
	JsonNode config;



	std::vector<Module*>::iterator item = modules.begin();

	BROFILER_CATEGORY("Engine PreUpdate", Profiler::Color::Yellow)
	for (; item != modules.end(); ++item)
	{
		(*item)->Save(&config);
	}
	LOG("Succesfully saved");
	wantToSave = false;
}

// Call PreUpdate, Update and PostUpdate on all modules
updateStatus Application::Update()
{
	updateStatus ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::vector<Module*>::iterator item = modules.begin();
	
	BROFILER_CATEGORY("Engine PreUpdate", Profiler::Color::Yellow)
	for(; item != modules.end() && ret == UPDATE_CONTINUE; ++item)
	{
		ret = (*item)->PreUpdate(dt);
	}

	item = modules.begin();

	BROFILER_CATEGORY("Engine Update", Profiler::Color::Green)
	for(;item != modules.end() && ret == UPDATE_CONTINUE; ++item)
	{
		ret = (*item)->Update(dt);
	}

	item = modules.begin();

	BROFILER_CATEGORY("Engine PostUpdate", Profiler::Color::Purple)
	for(;item != modules.end() && ret == UPDATE_CONTINUE; ++item)
	{
		ret = (*item)->PostUpdate(dt);
	}

	BROFILER_CATEGORY("Sleep", Profiler::Color::Blue)
	FinishUpdate();

	if (wantToExit)
		ret = updateStatus::UPDATE_STOP;

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	cleaningUp = true;

	for( auto item = modules.begin(); item != modules.end() && ret == true; ++item)
	{
		ret = (*item)->CleanUp();
		delete (*item);
	}

	modules.clear();

	delete frameTimer;
	delete secondsTimer;

	return ret;
}

const char* Application::GetTitle() const
{
	return title.c_str();
}

float Application::GetFrameCap() const
{
	return frameCap;
}

void Application::SwitchVsync()
{
	if (vsync)
		SDL_GL_SetSwapInterval(1);
	else
		SDL_GL_SetSwapInterval(0);
}

void Application::SetTitle(const char* _title)
{
	title = _title;
	App->window->SetTitle(_title);
}

void Application::SetFrameCap(int cap)
{
	frameCap = cap;
}

void Application::WantToSave()
{
	wantToSave = true;
}

void Application::ExitApp()
{
	wantToExit = true;
}

void Application::OpenLink(const char* link)
{
	ShellExecute(NULL, "open", link, NULL, NULL, SW_SHOWNORMAL);
}

std::string Application::ReadTxt(const char* path)
{
	std::string ret;
	FILE* fp;
	char str[MAXCHAR];

	const char* filename = path;

	fp = fopen(filename, "r");
	if (fp == nullptr) {
		LOG("COuld not read file %s", filename);
		return "Could not read License";
	}
	while (fgets(str, MAXCHAR, fp) != NULL)
		ret += str;

	fclose(fp);
	LOG("Read txt file %s", path);
	return ret;
}

void Application::AddModule(Module* mod)
{
	modules.push_back(mod);
}

bool Application::IsCleaningUp() const
{
	return cleaningUp;
}

Application* App = nullptr;
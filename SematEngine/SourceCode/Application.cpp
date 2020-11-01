#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleSceneIntro.h"
#include "ModuleWindow.h"
#include "Editor.h"

#include "Dependecies/Brofiler/Brofiler.h"

Application::Application() : debug(false), renderPrimitives(true), dt(0.16f)
{
	window = new ModuleWindow();
	input = new ModuleInput();
	scene_intro = new ModuleSceneIntro();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	editor = new Editor();

	// They will CleanUp() in reverse order
	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(editor);
	
	// Scenes
	AddModule(scene_intro);

	// Renderer last!
	AddModule(renderer3D);

	title = "Semat Engine";
	wantToExit = false;
	frame_cap = 120;
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
	
	frame_timer.Start();
	seconds_timer.Start();
	frame_cap = 120;

	vsync = VSYNC;

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)frame_timer.Read() / 1000.0f;
	frame_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	
	frame_cap_ms = 1000 / frame_cap;
	uint current_frame_ms = frame_timer.Read();

	if ( !vsync && current_frame_ms < frame_cap_ms)						
	{
		SDL_Delay(frame_cap_ms - current_frame_ms);
	}

	App->editor->UpdateConfigMS(frame_timer.Read());

	if (seconds_timer.Read() >= 1000)
	{
		seconds_timer.Start();
		App->editor->UpdateConfigFPS(frame_count);

		frame_count = 0;
	}

	frame_count++;

}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
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
		ret = update_status::UPDATE_STOP;

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for( auto item = modules.begin(); item != modules.end() && ret == true; ++item)
	{
		ret = (*item)->CleanUp();
	}
	return ret;
}

const char* Application::GetTitle() const
{
	return title.c_str();
}

float Application::GetFrameCap() const
{
	return frame_cap;
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
	frame_cap = cap;
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

	char* filename = "../../LICENSE.md";

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Could not open file %s", filename);
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

Application* App = nullptr;
#include "Application.h"
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
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{

	//Time::Update();
	float frame_ms = ms_timer.Read();
	
	/*if (frame_ms > 0 && frame_ms < 1000.f / frame_cap)
	{
		SDL_Delay((1000.f / frame_cap) - frame_ms);
	}*/

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

void Application::SetTitle(const char* _title)
{
	title = _title;
	App->window->SetTitle(_title);
}

void Application::AddModule(Module* mod)
{
	modules.push_back(mod);
}

Application* App = nullptr;
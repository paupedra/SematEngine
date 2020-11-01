#include "Globals.h"
#include "Editor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleSceneIntro.h"

#include <Windows.h>
#include "Dependecies/Glew/include/glew.h" // extension lib
#include "Dependecies/SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Dependecies/imgui/imgui.h"
#include "Dependecies/imgui/imgui_internal.h"
#include "Dependecies/imgui/imgui_impl_sdl.h"
#include "Dependecies/imgui/imgui_impl_opengl3.h"

#include "Window.h"
#include "w_Console.h"
#include "w_Configuration.h"
#include "w_About.h"
#include "w_Hierarchy.h"
#include "w_Inspector.h"


#include "Dependecies/mmgr/mmgr.h"

Editor::Editor(bool start_enabled) : Module(start_enabled)
{
	configuration = new w_Configuration(true);
	console = new w_Console(true);
	about = new w_About(false);
	hierarchy = new w_Hierarchy(true);
	inspector = new w_Inspector(true);

	AddWindow(configuration);
	AddWindow(console);
	AddWindow(about);
	AddWindow(hierarchy);
	AddWindow(inspector);
}

Editor::~Editor()
{

}

bool Editor::Start()
{
	ImGui::CreateContext();

	ImGui::GetIO().DisplaySize = ImVec2(1000, 1000);
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

	std::vector<Window*>::iterator item = windows.begin();
	for (item; item != windows.end(); ++item)
		(*item)->Init();

	SetHardwareInfo(configuration->GetHardwareInfo());

	return true;
}

update_status Editor::PreUpdate(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	SetUpDocking();

	return UPDATE_CONTINUE;
}

bool Editor::CleanUp()
{
	LOG("CleanUp Editor");

	std::vector<Window*>::iterator item = windows.begin();
	for (item; item != windows.end(); ++item)
		(*item)->CleanUp();

	//delete console;
	//delete configuration;
	//delete about;

	return true;
}

void Editor::Draw()
{
	//ImGui::CaptureKeyboardFromApp(true);

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save")) {  }
			if (ImGui::MenuItem("Load")) {  }
			if (ImGui::MenuItem("Exit")) { App->ExitApp(); }

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Console"," ",console->active)) { console->SetActive(); }
			if (ImGui::MenuItem("Configuration", " ", configuration->active)) { configuration->SetActive(); }
			if (ImGui::MenuItem("Hierarchy", " ", hierarchy->active)) { hierarchy->SetActive(); }
			if (ImGui::MenuItem("Inspector", " ", inspector->active)) { inspector->SetActive(); }

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Primitives"))
		{
			if (ImGui::MenuItem("Cube", " ", console->active)) {App->scene_intro->CreateGameObject("Cube Primitive", "Assets/Mesh/Cube.FBX", "");}
			if (ImGui::MenuItem("Cylinder", " ", console->active)) {App->scene_intro->CreateGameObject("Cylinder Primitive", "Assets/Mesh/Cylinder.FBX", "");}
			if (ImGui::MenuItem("Sphere", " ", console->active)) {App->scene_intro->CreateGameObject("Sphere Primitive", "Assets/Mesh/Sphere.FBX", "");}
			if (ImGui::MenuItem("Plane", " ", console->active)) {App->scene_intro->CreateGameObject("Plane Primitive", "Assets/Mesh/Plane.FBX", "");}
			if (ImGui::MenuItem("Pyramid", " ", console->active)) { App->scene_intro->CreateGameObject("Pyramid Primitive", "Assets/Mesh/Pyramid.FBX", ""); }
			if (ImGui::MenuItem("Teapot", " ", console->active)) { App->scene_intro->CreateGameObject("Teapot Primitive", "Assets/Mesh/Teapot.FBX", ""); }

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About", " ", about->active)) { about->SetActive(); }

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	std::vector<Window*>::iterator item = windows.begin();
	for (item; item != windows.end(); ++item)
		(*item)->Draw();

	ImGui::Render();
	
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void Editor::SetUpDocking()
{
	ImGuiDockNodeFlags dockFlags = ImGuiDockNodeFlags_PassthruCentralNode;

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_DockNodeHost ;
	windowFlags |= ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
	windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	//window that will hold the dock
	ImGui::Begin("Dock Window", &dockActive,windowFlags);

	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);

	ImGuiID id = ImGui::GetID("Dockspace");
	ImGui::DockSpace(id,ImVec2(0,0),dockFlags);
	
	ImGui::End();
}

void Editor::AddWindow(Window* window)
{
	windows.push_back(window);
}

void Editor::UpdateConfigFPS(int fps)
{
	configuration->UpdateFPS(fps);
}

void Editor::UpdateConfigMS(int ms)
{
	configuration->UpdateMS(ms);
}
void Editor::AddLog(char* text)
{
	console->AddLog(text);
}

void Editor::AddInput(const char* input)
{
	configuration->AddInput(input);
}

void Editor::SetHardwareInfo(HardwareInfo* hardware)
{
	hardware->cpuVendor = (const char*)glGetString(GL_VENDOR);
	hardware->renderer = (const char*)glGetString(GL_RENDERER);
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
}
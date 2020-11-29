#include <Windows.h>
#include "Globals.h"
#include "Application.h"
#include "Config.h"

#include "MEditor.h"
#include "MWindow.h"
#include "MRenderer3D.h"
#include "MScene.h"

#include "Window.h"
#include "WConsole.h"
#include "WConfiguration.h"
#include "WAbout.h"
#include "WHierarchy.h"
#include "WInspector.h"
#include "WAssets.h"

#include "IScene.h"

#include "Dependecies/Glew/include/glew.h" // extension lib
#include "Dependecies/SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Dependecies/imgui/imgui.h"
#include "Dependecies/imgui/imgui_internal.h"
#include "Dependecies/imgui/imgui_impl_sdl.h"
#include "Dependecies/imgui/imgui_impl_opengl3.h"

#include "Dependecies/mmgr/mmgr.h"

MEditor::MEditor(bool start_enabled) : Module(start_enabled)
{
	configuration = new WConfiguration(true);
	console = new WConsole(true);
	about = new WAbout(false);
	hierarchy = new WHierarchy(true);
	inspector = new WInspector(true);
	assets = new WAssets(true);

	AddWindow(configuration);
	AddWindow(console);
	AddWindow(about);
	AddWindow(hierarchy);
	AddWindow(inspector);
	AddWindow(assets);
}

MEditor::~MEditor()
{

}

bool MEditor::Start()
{
	LOG("Creating ImGui Context");
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

update_status MEditor::PreUpdate(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	SetUpDocking();

	return UPDATE_CONTINUE;
}

bool MEditor::CleanUp()
{
	LOG("CleanUp Editor");

	std::vector<Window*>::iterator item = windows.begin();
	for (item; item != windows.end(); ++item)
		(*item)->CleanUp();

	delete console;
	delete configuration;
	delete about;
	delete hierarchy;
	delete inspector;

	windows.clear();

	return true;
}

bool MEditor::Save(ConfigNode* config)
{
	LOG("saved editor");

	return true;
}

void MEditor::Draw()
{
	//ImGui::CaptureKeyboardFromApp(true);

	ImGuiIO io = ImGui::GetIO();

	DrawMainMenuBar();

	std::vector<Window*>::iterator item = windows.begin();
	for (item; item != windows.end(); ++item)
		(*item)->Draw();

	ImGui::Render();

	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	glClearColor(0, 0, 0, 0);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); //Crashes when loading buffers of meshes

}

void MEditor::DrawMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save")) { App->scene->SaveScene(); }
			if (ImGui::MenuItem("Exit")) { App->ExitApp(); }

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Console", " ", console->active)) { console->SetActive(); }
			if (ImGui::MenuItem("Configuration", " ", configuration->active)) { configuration->SetActive(); }
			if (ImGui::MenuItem("Hierarchy", " ", hierarchy->active)) { hierarchy->SetActive(); }
			if (ImGui::MenuItem("Inspector", " ", inspector->active)) { inspector->SetActive(); }

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Primitives"))
		{
			if (ImGui::MenuItem("Cube", " ", console->active)) { Importer::SceneImporter::Import( "Assets/Mesh/Primitives/Cube.FBX"); }
			if (ImGui::MenuItem("Cylinder", " ", console->active)) { Importer::SceneImporter::Import("Assets/Mesh/Primitives/Cylinder.FBX"); }
			if (ImGui::MenuItem("Sphere", " ", console->active)) { Importer::SceneImporter::Import("Assets/Mesh/Primitives/Sphere.FBX"); }
			if (ImGui::MenuItem("Plane", " ", console->active)) { Importer::SceneImporter::Import("Assets/Mesh/Primitives/Plane.FBX"); }
			if (ImGui::MenuItem("Pyramid", " ", console->active)) { Importer::SceneImporter::Import("Assets/Mesh/Primitives/Pyramid.FBX"); }
			if (ImGui::MenuItem("Teapot", " ", console->active)) { Importer::SceneImporter::Import("Assets/Mesh/Primitives/Teapot.FBX"); }

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About", " ", about->active)) { about->SetActive(); }

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void MEditor::SetUpDocking()
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

void MEditor::AddWindow(Window* window)
{
	windows.push_back(window);
}

void MEditor::UpdateConfigFPS(int fps)
{
	configuration->UpdateFPS(fps);
}

void MEditor::UpdateConfigMS(int ms)
{
	configuration->UpdateMS(ms);
}
void MEditor::AddLog(char* text)
{
	console->AddLog(text);
}

void MEditor::AddInput(const char* input)
{
	configuration->AddInput(input);
}

void MEditor::SetHardwareInfo(HardwareInfo* hardware)
{
	hardware->cpuVendor = (const char*)glGetString(GL_VENDOR);
	hardware->renderer = (const char*)glGetString(GL_RENDERER);
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
}
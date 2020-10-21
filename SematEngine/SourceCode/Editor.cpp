#include "Globals.h"
#include "Editor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

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

#include "w_Configuration.h"
#include "w_Console.h"
#include "w_About.h"

#include "Dependecies/mmgr/mmgr.h"

Editor::Editor(bool start_enabled) : Module(start_enabled)
{
	configuration = new w_Configuration(false);
	console = new w_Console(false);
	about = new w_About(false);

	AddWindow(configuration);
	AddWindow(console);
	AddWindow(about);
}

Editor::~Editor()
{

}

bool Editor::Start()
{
	ImGui::CreateContext();

	ImGui::GetIO().DisplaySize = ImVec2(1000, 1000);

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();
	return true;
}

update_status Editor::PreUpdate(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

bool Editor::CleanUp()
{
	LOG("CleanUp Editor");

	std::vector<Window*>::iterator item = windows.begin();
	for (item; item != windows.end(); ++item)
		(*item)->CleanUp();

	delete console;
	delete configuration;
	delete about;

	return true;
}

void Editor::Draw()
{
	/*if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("fill de puta"))
		{
			if (ImGui::BeginMenu("malparit"))
			{

			}
		}
	}

	ImGui::EndMainMenuBar();*/

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

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y")) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
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
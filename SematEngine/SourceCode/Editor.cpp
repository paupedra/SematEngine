#include "Globals.h"
#include "Editor.h"
#include "Application.h"

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

Editor::Editor(bool start_enabled) : Module(start_enabled)
{
	configuration = new w_Configuration(true);
	console = new w_Console(true);

	AddWindow(configuration);
	AddWindow(console);
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
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Console","fill de puta",console->active)) { console->SetActive(); }
			if (ImGui::MenuItem("Configuration", "fill de puta 2", configuration->active)) { configuration->SetActive(); }

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
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

void Editor::AddWindow(Window* _window)
{
	windows.push_back(_window);
}
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

Editor::Editor(bool start_enabled) : Module(start_enabled)
{

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
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
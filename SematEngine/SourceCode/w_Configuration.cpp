#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "Window.h"

#include "w_Configuration.h"

#include "Dependecies/imgui/imgui.h"

#include <string>

#include "Dependecies/mmgr/mmgr.h"

w_Configuration::w_Configuration(bool _active) : Window(_active)
{

}

w_Configuration::~w_Configuration()
{

}

void w_Configuration::Draw()
{
	if (!active)
		return;
	
	if (ImGui::Begin("Configuration", &active))
	{
		if (ImGui::CollapsingHeader("Application"))
		{
			char appName[100];
			strcpy_s(appName, 100, App->GetTitle());

			if (ImGui::InputText("Project Name", appName, 100, ImGuiInputTextFlags_EnterReturnsTrue))
				App->SetTitle(appName);

			//Parida de frames
			fps = App->GetFrameCap();
			std::string fpsString = "FPS = " + fps;

			ImGui::PlotHistogram("FPS", FPS, IM_ARRAYSIZE(FPS), 0, fpsString.c_str() , 0.0f, 144.0f, ImVec2(0, 80));
			if (ImGui::SliderInt("FPS", &fps, 5, 144)) { App->SetFrameCap(fps); }

			ImGui::PlotHistogram("MS", MS, IM_ARRAYSIZE(MS), 0, NULL, 0.0f, 100.0f, ImVec2(0, 80));
		}

		if (ImGui::CollapsingHeader("Window"))
		{
			int w = App->window->GetWidth();
			int h = App->window->GetHeight();
			if (ImGui::SliderInt("Width", &w, 0, 1920)) { App->window->SetWidth(w); }
			if (ImGui::SliderInt("Height", &h, 0, 1080)) { App->window->SetHeight(h); }
		}

		if (ImGui::CollapsingHeader("Render"))
		{
			if (ImGui::Checkbox("Vsync", &App->vsync )) { App->SwitchVsync(); }
			if (ImGui::Checkbox("Wireframe Mode", &App->renderer3D->wireframeMode)) {  }
			if (ImGui::Checkbox("GL_CULL_FACE", &App->renderer3D->glCullFace)) { App->renderer3D->SwitchCullFace(); }
			if (ImGui::Checkbox("GL_DEPTH_TEST", &App->renderer3D->glDepthTest)) { App->renderer3D->SwitchDepthTest(); }
			if (ImGui::Checkbox("GL_LIGHT", &App->renderer3D->glLighting)) { App->renderer3D->SwitchLighting(); }
			if (ImGui::Checkbox("GL_COLOR_MATERIAL", &App->renderer3D->glColorMaterial)) { App->renderer3D->SwitchColorMaterial(); }
			if (ImGui::Checkbox("GL_TEXTURE_2D", &App->renderer3D->glTexture2d)) { App->renderer3D->SwitchTexture2d(); }
		}

		if (ImGui::CollapsingHeader("Input"))
		{
			ImGui::Text("Mouse Position: %i,%i", App->input->GetMouseX(), App->input->GetMouseY());
			ImGui::Text("Mouse motion: %i, %i", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());
			ImGui::Text("Mouse wheel: %i", App->input->GetMouseZ());
			
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
				
			ImGui::BeginChild("Input", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 260), false, window_flags);
			for (int i = 0; i < 100; i++)
				ImGui::Text("%04d: scrollable region", i);

			ImGui::EndChild();
			
		}

		if (ImGui::CollapsingHeader("Audio"))
		{

		}

		if (ImGui::CollapsingHeader("Hardware"))
		{

		}

		ImGui::End();
	}
}

void w_Configuration::UpdateFPS(int _FPS)
{
	for (int i = 0; i < 20 - 1; i++)
	{
		FPS[i] = FPS[i + 1];
	}

	FPS[19] = _FPS;
}

void w_Configuration::UpdateMS(int _ms)
{
	for (int i = 0; i < 20 - 1; i++)
	{
		MS[i] = MS[i + 1];
	}

	MS[19] = _ms;

}

void w_Configuration::CleanUp()
{

}
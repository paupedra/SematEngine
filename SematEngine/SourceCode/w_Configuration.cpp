#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
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

			fps = App->GetFrameCap();
			std::string fpsString = "FPS = ";
			fpsString += (fps + '0');

			ImGui::PlotHistogram("FPS", FPS, IM_ARRAYSIZE(FPS), 0, fpsString.c_str() , 0.0f, 144.0f, ImVec2(0, 80));
			if (ImGui::SliderInt("FPS", &fps, 5, 144)) { App->SetFrameCap(fps); }

			ImGui::PlotHistogram("FPS", MS, IM_ARRAYSIZE(MS), 0, NULL, 0.0f, 100.0f, ImVec2(0, 80));
		}

		if (ImGui::CollapsingHeader("Window"))
		{
			
		}

		if (ImGui::CollapsingHeader("Render"))
		{
			if (ImGui::Checkbox("Vsync", &App->vsync )) { App->SwitchVsync(); }
			if (ImGui::Checkbox("GL_CULL_FACE", &App->renderer3D->cullFace)) { App->renderer3D->SwitchCullFace(); }
		}

		if (ImGui::CollapsingHeader("Input"))
		{
			ImGui::Text("Mouse Position: %i,%i", App->input->GetMouseX(), App->input->GetMouseY());
			ImGui::Text("Mouse motion: %i, %i", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());
			ImGui::Text("Mouse wheel: %i", App->input->GetMouseZ());
		}

		if (ImGui::CollapsingHeader("Audio"))
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
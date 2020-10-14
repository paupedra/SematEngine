#include "Application.h"

#include "w_Configuration.h"

#include "Dependecies/imgui/imgui.h"

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
		if (ImGui::BeginMenu(""))
		{

			ImGui::EndMenu();
		}

		if (ImGui::CollapsingHeader("Application"))
		{
			char appName[100];
			strcpy_s(appName, 100, App->GetTitle());

			if (ImGui::InputText("Project Name", appName, 100, ImGuiInputTextFlags_EnterReturnsTrue))
				App->SetTitle(appName);
		}

		if (ImGui::CollapsingHeader("Window"))
		{

		}

		if (ImGui::CollapsingHeader("Render"))
		{

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
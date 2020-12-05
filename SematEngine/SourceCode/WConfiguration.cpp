#include <string>
#include "Application.h"
#include "Window.h"

#include "MInput.h"
#include "MRenderer3D.h"
#include "MWindow.h"

#include "WConfiguration.h"

#include "Dependecies/SDL/include/SDL.h"

#include "Dependecies/imgui/imgui.h"

#include "Dependecies/mmgr/mmgr.h"

WConfiguration::WConfiguration(bool _active) : Window(_active)
{
	FPS[19] = 0;
}

WConfiguration::~WConfiguration()
{

}

void WConfiguration::Init()
{
	hardware.cacheLineSize =  SDL_GetCPUCacheLineSize();
	hardware.cpuCount = SDL_GetCPUCount();
	hardware.ram = SDL_GetSystemRAM() / 1000;
	fps = App->GetFrameCap();
}

void WConfiguration::Draw()
{
	if (!active)
		return;
	
	if (!ImGui::Begin("Configuration", &active))
	{
		ImGui::End();
		return;
	}

	DrawApplication();
	DrawWindow();
	DrawRender();
	DrawInput();
	DrawHardware();
	
	ImGui::End();
}

void WConfiguration::DrawApplication()
{
	if (ImGui::CollapsingHeader("Application"))
	{
		char appName[100];
		strcpy_s(appName, 100, App->GetTitle());

		if (ImGui::InputText("Project Name", appName, 100, ImGuiInputTextFlags_EnterReturnsTrue))
			App->SetTitle(appName);

		//Frames to char, very ugly
		char string[10];
		sprintf(string, "%.2f", FPS[19]);
		char fpsStr[20] = "FPS: ";
		strcat(fpsStr, string);

		ImGui::PlotHistogram("FPS", FPS, IM_ARRAYSIZE(FPS), 0, fpsStr, 0.0f, 144.0f, ImVec2(0, 80));
		if (ImGui::SliderInt("FPS Slider", &fps, 5, 144)) { App->SetFrameCap(fps); }

		sprintf(string, "%.2f", MS[19]);
		char msStr[20] = "MS: ";
		strcat(msStr, string);

		ImGui::PlotHistogram("MS", MS, IM_ARRAYSIZE(MS), 0, msStr, 0.0f, 100.0f, ImVec2(0, 80));
	}
}

void WConfiguration::DrawWindow()
{
	if (ImGui::CollapsingHeader("Window"))
	{
		int w = App->window->GetWidth();
		int h = App->window->GetHeight();
		if (ImGui::SliderInt("Width", &w, 0, 1920)) { App->window->SetWidth(w); }
		if (ImGui::SliderInt("Height", &h, 0, 1080)) { App->window->SetHeight(h); }
	}
}

void WConfiguration::DrawRender()
{
	if (ImGui::CollapsingHeader("Render"))
	{
		if (ImGui::Checkbox("Vsync", &App->vsync)) { App->SwitchVsync(); }
		if (ImGui::Checkbox("Wireframe Mode", &App->renderer3D->wireframeMode)) {}
		if (ImGui::Checkbox("GL_CULL_FACE", &App->renderer3D->glCullFace)) { App->renderer3D->SwitchCullFace(); }
		if (ImGui::Checkbox("GL_DEPTH_TEST", &App->renderer3D->glDepthTest)) { App->renderer3D->SwitchDepthTest(); }
		if (ImGui::Checkbox("GL_LIGHT", &App->renderer3D->glLighting)) { App->renderer3D->SwitchLighting(); }
		if (ImGui::Checkbox("GL_COLOR_MATERIAL", &App->renderer3D->glColorMaterial)) { App->renderer3D->SwitchColorMaterial(); }
		if (ImGui::Checkbox("GL_TEXTURE_2D", &App->renderer3D->glTexture2d)) { App->renderer3D->SwitchTexture2d(); }
		if (ImGui::Checkbox("Draw All Bounding Boxes", &App->renderer3D->drawAllBoundingBoxes)) {}
	}
}

void WConfiguration::DrawInput()
{
	if (ImGui::CollapsingHeader("Input"))
	{
		ImGui::Text("Mouse Position: %i,%i", App->input->GetMouseX(), App->input->GetMouseY());
		ImGui::Text("Mouse motion: %i, %i", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());
		ImGui::Text("Mouse wheel: %i", App->input->GetMouseZ());

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;

		ImGui::BeginChild("Input", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 260), false, window_flags);

		std::vector<const char*>::iterator item = inputs.begin();
		for (; item != inputs.end(); ++item)
			ImGui::Text((*item));

		if (scrollToBottomInputs)
		{
			ImGui::SetScrollHere(1.0f);
			scrollToBottomInputs = false;
		}
		ImGui::EndChild();
	}
}

void WConfiguration::DrawHardware()
{
	if (ImGui::CollapsingHeader("Hardware Detection and Software Versions"))
	{
		ImGui::Text("Cpu Count: %d cores", hardware.cpuCount);
		ImGui::Text("Cache Line Size: %d bytes", hardware.cacheLineSize);
		ImGui::Text("RAM: %d mp", hardware.ram);
		ImGui::Text("GPU vendor: %s", hardware.cpuVendor.c_str());
		ImGui::Text("Renderer: %s", hardware.renderer.c_str());
	}
}

void WConfiguration::UpdateFPS(int _FPS)
{
	for (int i = 0; i < 20 - 1; i++) //Magic
	{
		FPS[i] = FPS[i + 1];
	}

	FPS[19] = _FPS;
}

void WConfiguration::UpdateMS(int _ms)
{
	for (int i = 0; i < 20 - 1; i++) //Magic
	{
		MS[i] = MS[i + 1];
	}

	MS[19] = _ms;
}

void WConfiguration::AddInput(const char* input)
{
	inputs.push_back(strdup(input));
	if (inputs.size() > 20)
	{
		inputs.erase(inputs.begin());
	}
	scrollToBottomInputs = true;
}

void WConfiguration::CleanUp()
{
	inputs.clear();
}

HardwareInfo* WConfiguration::GetHardwareInfo()
{
	return &hardware;
}
#include "Application.h"
#include "Window.h"

#include "MEditor.h"

#include "WPlay.h"

#include "Dependecies/imgui/imgui.h"

//#include "Dependecies/mmgr/mmgr.h"

WPlay::WPlay(bool _active) : Window(_active)
{

}

WPlay::~WPlay()
{

}

void WPlay::Init()
{

}

void WPlay::Draw()
{
	if (!active)
		return;

	if (!ImGui::Begin("Play", &active))
	{
		ImGui::End();
		return;
	}

	if (ImGui::Button("Play"))
	{
		App->Play();
	}
	ImGui::SameLine();
	if (ImGui::Button("Pause"))
	{
		App->Pause();
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop"))
	{
		App->Stop();
	}
	ImGui::SameLine();

	ImGui::Text("Play Time: %f", App->GetPlayTime());

	ImGui::SameLine();

	float timeMultiplier = App->GetTimeMultiplier();
	if (ImGui::DragFloat("Time Multiplier",&timeMultiplier,0.1,0.1,5))
	{
		App->SetTimeMultiplier(timeMultiplier);
	}
	
	if (ImGui::IsWindowHovered())
		App->editor->mouseHovered = true;
	ImGui::End();
}

void WPlay::CleanUp()
{

}
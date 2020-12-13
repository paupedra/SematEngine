#include "Application.h"
#include "Window.h"

#include "MEditor.h"

#include "WAbout.h"

#include "Dependecies/imgui/imgui.h"

#include "Dependecies/mmgr/mmgr.h"

WAbout::WAbout(bool _active) : Window(_active)
{
	licenseString = App->ReadTxt("../LICENSE.md");
}

WAbout::~WAbout()
{

}

void WAbout::Init()
{

}

void WAbout::Draw()
{
	if (!active)
		return;

	if (!ImGui::Begin("About Semat Engine", &active))
	{
		ImGui::End();
		return;
	}

	ImGui::Text("Engine Name: Semat Engine");

	if (ImGui::CollapsingHeader("Description"))
	{
		ImGui::TextWrapped("-Semat Engine is a small Game Engine developed in C++ by one student in a Bachelor's degree in Video Game Design and Development in CITM Barcelona.");
	}

	if (ImGui::CollapsingHeader("Author and Repository"))
	{
		ImGui::Text("Project by Pau Pedra Bonifacio");
		if (ImGui::Button("Project Repository")) { App->OpenLink("https://github.com/paupedra/SematEngine"); };
	}
	if (ImGui::CollapsingHeader("License"))
	{
		
		ImGui::TextWrapped(licenseString.c_str());
	}
	if (ImGui::IsWindowHovered())
		App->editor->mouseHovered = true;


	ImGui::End();
	
}

void WAbout::CleanUp()
{

}
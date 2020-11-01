#include "Application.h"

#include "Window.h"

#include "w_About.h"

#include "Dependecies/imgui/imgui.h"

#include "Dependecies/mmgr/mmgr.h"

w_About::w_About(bool _active) : Window(_active)
{
	licenseString = App->ReadTxt("../../LICENSE.md");
}

w_About::~w_About()
{

}

void w_About::Init()
{

}

void w_About::Draw()
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

	ImGui::End();
	
}

void w_About::CleanUp()
{

}
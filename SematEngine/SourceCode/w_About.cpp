#include "Application.h"

#include "w_About.h"

#include "Dependecies/imgui/imgui.h"

#include "Dependecies/mmgr/mmgr.h"

w_About::w_About(bool _active) : Window(_active)
{
	//licenseString = App->ReadTxt("../../LICENSE.txt");
	licenseString = "Licence goes here";
}

w_About::~w_About()
{

}

void w_About::Draw()
{
	if (!active)
		return;

	if (ImGui::Begin("About Semat Engine", &active))
	{
		ImGui::Text("Engine Name: Semat Engine");

		if (ImGui::CollapsingHeader("Description"))
		{
			ImGui::Text("");
		}

		if (ImGui::CollapsingHeader("Author and Repository"))
		{
			ImGui::Text("Project by Pau Pedra Bonifacio");
			if (ImGui::Button("Project Repository")) { App->OpenLink("https://github.com/paupedra/SematEngine"); };
		}
		if (ImGui::CollapsingHeader("License"))
		{
			
			ImGui::Text(licenseString);
		}

		ImGui::End();
	}
}

void w_About::CleanUp()
{

}
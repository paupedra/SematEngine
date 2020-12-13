#include "Application.h"
#include "Window.h"
#include "Resource.h"

#include "WResources.h"

#include "MResourceManager.h"

#include "Dependecies/imgui/imgui.h"

#include "Dependecies/mmgr/mmgr.h"

WResources::WResources(bool _active) : Window(_active)
{

}

WResources::~WResources()
{

}

void WResources::Init()
{

}

void WResources::Draw()
{
	if (!active)
		return;

	if (!ImGui::Begin("Resources", &active))
	{
		ImGui::End();
		return;
	}

	ImGui::Text("Resources in Library:");

	std::map<UID, ResourceData>::iterator resourceLib = App->resourceManager->resourcesInLibrary.begin();
	for (; resourceLib != App->resourceManager->resourcesInLibrary.end(); resourceLib++)
	{
		if (ImGui::Button((*resourceLib).second.assetsFile.c_str()))
		{
			LOG("Load resource %s", (*resourceLib).second.assetsFile.c_str());
			App->resourceManager->LoadResource((*resourceLib).second.UID);
		}
	}

	ImGui::Separator();

	ImGui::Text("Resources in Memory:");

	std::map<UID, Resource*>::iterator resource = App->resourceManager->resources.begin();
	for (; resource != App->resourceManager->resources.end(); resource++)
	{
		ImGui::Separator();
		ImGui::Text("Name: %s", (*resource).second->resourceData.libraryFile.c_str());
		ImGui::Text("Reference Count: %d", (*resource).second->resourceData.referenceCount);
	}

	ImGui::End();
}

void WResources::CleanUp()
{

}
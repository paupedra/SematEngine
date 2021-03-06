#include "Application.h"
#include "Window.h"
#include "Resource.h"

#include "MFileSystem.h"
#include "MResourceManager.h"
#include "MEditor.h"
#include "WAssets.h"

#include "ITexture.h"

#include "RTexture.h"

#include "Dependecies/Glew/include/glew.h"

#include "Dependecies/imgui/imgui.h"

//#include "Dependecies/mmgr/mmgr.h"

WAssets::WAssets(bool _active) : Window(_active)
{
	
}

WAssets::~WAssets()
{

}

void WAssets::Init()
{
	//folder = Importer::TextureImp::Import("Assets/Icons/folder.png");
}

void WAssets::Draw()
{
	if (!active)
		return;

	if (!ImGui::Begin("Assets", &active))
	{
		ImGui::End();
		return;
	}

	if(ImGui::BeginChild("Assets", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, ImGui::GetWindowHeight()), true))
	{
		DrawFolders("Assets");
	}
	if (ImGui::IsWindowHovered())
		App->editor->mouseHovered = true;

	ImGui::EndChild();

	ImGui::SameLine();
	
	if (ImGui::BeginChild("Project", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f,ImGui::GetWindowHeight()), true))
	{
		DrawFilesInFolder(selectedFolder.c_str());
	}
	if (ImGui::IsWindowHovered())
		App->editor->mouseHovered = true;
	
	ImGui::EndChild();

	if (ImGui::IsWindowHovered())
		App->editor->mouseHovered = true;

	ImGui::End();

}

void WAssets::DrawFolders(std::string path)
{
	std::vector<std::string> files;
	std::vector<std::string> dirs;

	App->fileSystem->DiscoverFiles(path.c_str(), files, dirs);

	ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	if (dirs.empty())
		baseFlags |= ImGuiTreeNodeFlags_Leaf;
	
	std::string name;
	name = path.substr(path.find_last_of("/") + 1); //Get folder's name only

	if (ImGui::TreeNodeEx(name.c_str(), baseFlags))
	{
		if (ImGui::IsItemClicked())
		{
			selectedFolder = path;
			LOG("Selected folder: %s", selectedFolder.c_str());
		}

		std::vector<std::string>::iterator it = dirs.begin();
		for (; it != dirs.end(); it++)
		{
			std::string newPath = path;
			newPath += "/" + (*it);
			DrawFolders(newPath.c_str());
		}

		ImGui::TreePop();
	}
}

void WAssets::DrawFolder(const char* name)
{

}

void WAssets::DrawFilesInFolder(const char* path)
{
	std::vector<std::string> files;
	std::vector<std::string> dirs;

	App->fileSystem->DiscoverFiles(path, files, dirs);

	std::vector<std::string>::iterator it = dirs.begin();
	for (; it != dirs.end(); it++)
	{
		//ImGui::SameLine();
		if (ImGui::Button((*it).c_str()))
		{
			selectedFolder = path;
			selectedFolder += "/";
			selectedFolder += (*it);

		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	it = files.begin();
	for (; it != files.end(); it++)
	{
		//ImGui::SameLine();
		if(ImGui::Button((*it).c_str() ) )
		{
			if (strstr((*it).c_str(), ".meta") != nullptr) //only fo this with files that are not metas
			{
				std::string metaName = path;
				metaName += "/";
				metaName += (*it);
				LOG("Load resource from meta %s", metaName.c_str());
				App->resourceManager->LoadResourceFromMeta(metaName);
			}
		}

		//ImGui::Text((*it).c_str());
	}
}

void DrawFile(const char* name) 
{

}

void WAssets::CleanUp()
{

}
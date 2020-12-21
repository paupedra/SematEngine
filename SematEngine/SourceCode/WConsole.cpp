#include "Application.h"
#include "Window.h"

#include "MEditor.h"

#include "WConsole.h"

#include "Dependecies/imgui/imgui.h"

//#include "Dependecies/mmgr/mmgr.h"

WConsole::WConsole(bool _active) : Window(_active)
{

}

WConsole::~WConsole()
{

}

void WConsole::Init()
{

}

void WConsole::Draw()
{
	if (!active)
		return;

	if (!ImGui::Begin("Console", &active))
	{
		ImGui::End();
		return;
	}

	std::vector<Logs>::iterator item = logs.begin();
	for (item; item != logs.end(); ++item)
	{
		ImVec4 textColor = { 1.f,1.f,1.f,1.f };

		switch ((*item).type)
		{
			case LogType::ERRORS:
				textColor = { 1.f,0.f,0.f,1.f };
				break;

			case LogType::INIT:
				textColor = { 1.f,1.f,0.f,1.f };
				break;

			case LogType::FILESYSTEM:
				textColor = { 0.f,0.f,1.f,1.f };
				break;
		}

		ImGui::PushStyleColor(ImGuiCol_Text, textColor);
		ImGui::TextUnformatted((*item).text);
		ImGui::PopStyleColor();
	}

	if (scrollToBottom)
	{
		ImGui::SetScrollHere(1.0f);
		scrollToBottom = false;
	}
	if (ImGui::IsWindowHovered())
		App->editor->mouseHovered = true;

	ImGui::End();
}

void WConsole::AddLog(char* text)
{
	Logs log;
	log.text = strdup(text);
	log.type = LogType::NONE;

	if (strstr(text, "(ERROR)") != nullptr)
	{
		log.type = LogType::ERRORS;
	}
	if (strstr(text, "(INIT)") != nullptr)
	{
		log.type = LogType::INIT;
	}
	if (strstr(text, "(FILESYSTEM)") != nullptr)
	{
		log.type = LogType::FILESYSTEM;
	}

	logs.push_back(log); //Why does this work?
	scrollToBottom = true;
	
}

void WConsole::CleanUp()
{
	logs.clear();

}
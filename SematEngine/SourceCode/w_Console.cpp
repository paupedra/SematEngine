#include "Application.h"

#include "Window.h"
#include "w_Console.h"

#include "Dependecies/imgui/imgui.h"

#include "Dependecies/mmgr/mmgr.h"

w_Console::w_Console(bool _active) : Window(_active)
{
	scrollToBottom = false;
}

w_Console::~w_Console()
{

}

void w_Console::Init()
{

}

void w_Console::Draw()
{
	if (!active)
		return;

	if (!ImGui::Begin("Console", &active))
	{
		ImGui::End();
		return;
	}

	std::vector<Logs*>::iterator item = logs.begin();
	for (item; item != logs.end(); ++item)
	{
		ImVec4 textColor = { 1.f,1.f,1.f,1.f };

		switch ((*item)->type)
		{
			case LogType::ERRORS:
				textColor = { 1.f,0.f,0.f,1.f };
				break;

			case LogType::INIT:
				textColor = { 1.f,1.f,0.f,1.f };
				break;
		}

		ImGui::PushStyleColor(ImGuiCol_Text, textColor);
		ImGui::TextUnformatted((*item)->text);
		ImGui::PopStyleColor();
	}

	if (scrollToBottom)
	{
		ImGui::SetScrollHere(1.0f);
		scrollToBottom = false;
	}

	ImGui::End();
}

void w_Console::AddLog(char* text)
{
	Logs* log = new Logs;
	log->text = strdup(text);
	log->type = LogType::NONE;

	if (strstr(text, "(ERROR)") != nullptr)
	{
		log->type = LogType::ERRORS;
	}
	if (strstr(text, "(INIT)") != nullptr)
	{
		log->type = LogType::INIT;
	}

	logs.push_back(log); //Why does this work?
	scrollToBottom = true;
	
}

void w_Console::CleanUp()
{

}
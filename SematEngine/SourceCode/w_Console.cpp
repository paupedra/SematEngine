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

void w_Console::Draw()
{

	if (!active)
		return;
	if (ImGui::Begin("Console", &active))
	{
		std::vector<char*>::iterator item = logs.begin();

		for (item; item != logs.end(); ++item)
		{
			ImGui::TextUnformatted((*item));
		}

		if (scrollToBottom)
		{
			ImGui::SetScrollHere(1.0f);
			scrollToBottom = false;
		}

		ImGui::End();
	}
}

void w_Console::AddLog(char* text)
{
	logs.push_back(strdup(text)); //Why does this work?
	scrollToBottom = true;
	
}

void w_Console::CleanUp()
{

}
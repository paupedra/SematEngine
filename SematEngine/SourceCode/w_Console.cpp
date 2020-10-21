#include "Application.h"
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
			ImGui::SetScrollY(ImGui::GetScrollMaxY());
			scrollToBottom = false;
		}

		ImGui::End();
	}
}

void w_Console::AddLog(char* text)
{
	logs.push_back(text);
	scrollToBottom = true;
}

void w_Console::CleanUp()
{

}
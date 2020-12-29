#include "Window.h"
#include "WGame.h"

#include "Dependecies/imgui/imgui.h"

WGame::WGame(bool _active) : Window(_active)
{

}

WGame::~WGame()
{

}

void WGame::Init()
{

}

void WGame::Draw()
{
	if (!active)
		return;

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize ;
	windowFlags |= ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar ;
	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	windowFlags |= ImGuiWindowFlags_NoCollapse;

	if (!ImGui::Begin("Game", &active,windowFlags))
	{
		ImGui::End();
		return;
	}

	ImGui::End();
}

void WGame::DrawTree(GameObject* object)
{

}

void WGame::CleanUp()
{

}
#include "Application.h"
#include "Window.h"

#include "WPlay.h"

#include "Dependecies/imgui/imgui.h"

#include "Dependecies/mmgr/mmgr.h"

WPlay::WPlay(bool _active) : Window(_active)
{

}

WPlay::~WPlay()
{

}

void WPlay::Init()
{

}

void WPlay::Draw()
{
	if (!active)
		return;

	if (!ImGui::Begin("Play", &active))
	{
		ImGui::End();
		return;
	}

	if (ImGui::Button("Play"))
	{

	}
	

	ImGui::End();
}

void WPlay::CleanUp()
{

}
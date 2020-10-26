#include "Application.h"

#include "Window.h"

#include "w_Inspector.h"

#include "Dependecies/imgui/imgui.h"

#include "Dependecies/mmgr/mmgr.h"

w_Inspector::w_Inspector(bool _active) : Window(_active)
{

}

w_Inspector::~w_Inspector()
{

}

void w_Inspector::Draw()
{
	if (!active)
		return;

	if (ImGui::Begin("Inspector", &active))
	{

		ImGui::End();
	}
}

void w_Inspector::CleanUp()
{

}
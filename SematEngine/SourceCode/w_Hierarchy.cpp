#include "Application.h"

#include "Window.h"

#include "w_Hierarchy.h"

#include "Dependecies/imgui/imgui.h"

#include "Dependecies/mmgr/mmgr.h"

w_Hierarchy::w_Hierarchy(bool _active) : Window(_active)
{

}

w_Hierarchy::~w_Hierarchy()
{

}

void w_Hierarchy::Draw()
{
	if (!active)
		return;

	if (ImGui::Begin("Hierarchy", &active))
	{
		
		ImGui::End();
	}
}

void w_Hierarchy::CleanUp()
{

}
#include "Application.h"

#include "Window.h"

#include "w_Inspector.h"

#include "ModuleSceneIntro.h"

#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"

#include "Dependecies/imgui/imgui.h"

#include "Dependecies/mmgr/mmgr.h"

w_Inspector::w_Inspector(bool _active) : Window(_active)
{

}

w_Inspector::~w_Inspector()
{

}

void w_Inspector::Init()
{

}

void w_Inspector::Draw()
{
	if (!active)
		return;

	if (!ImGui::Begin("Inspector", &active))
	{
		ImGui::End();
		return;
	}

	if (App->scene_intro->selectedObject != nullptr)
	{
		std::vector<Component*> vector = App->scene_intro->selectedObject->GetComponents();
		std::vector<Component*>::iterator item = vector.begin();
		for (; item != vector.end(); ++item)
		{
			(*item)->DrawInspector();
		}
	}

	ImGui::End();
	
}

void w_Inspector::CleanUp()
{

}
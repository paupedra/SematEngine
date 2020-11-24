#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "Window.h"

#include "MScene.h"

#include "CTransform.h"

#include "WInspector.h"

#include "Dependecies/imgui/imgui.h"
#include "Dependecies/mmgr/mmgr.h"

WInspector::WInspector(bool _active) : Window(_active)
{

}

WInspector::~WInspector()
{

}

void WInspector::Init()
{

}

void WInspector::Draw()
{
	if (!active)
		return;

	if (!ImGui::Begin("Inspector", &active))
	{
		ImGui::End();
		return;
	}

	if (App->scene->selectedObject != nullptr)
	{
		std::vector<Component*> vector = App->scene->selectedObject->GetComponents();
		std::vector<Component*>::iterator item = vector.begin();
		for (; item != vector.end(); ++item)
		{
			(*item)->DrawInspector();
		}
	}

	ImGui::End();
	
}

void WInspector::CleanUp()
{

}
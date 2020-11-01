#include "Application.h"

#include "Window.h"

#include "w_Hierarchy.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"

#include "Dependecies/imgui/imgui.h"

#include "Dependecies/mmgr/mmgr.h"

w_Hierarchy::w_Hierarchy(bool _active) : Window(_active)
{

}

w_Hierarchy::~w_Hierarchy()
{

}

void w_Hierarchy::Init()
{

}

void w_Hierarchy::Draw()
{
	if (!active)
		return;

	if (!ImGui::Begin("Hierarchy", &active))
	{
		ImGui::End();
		return;
	}

	
	ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	std::vector<GameObject*>::iterator item = App->scene_intro->gameObjects.begin();
	for (; item != App->scene_intro->gameObjects.end(); ++item)
		DrawTree((*item));
		
	ImGui::End();
	
}

void w_Hierarchy::DrawTree(GameObject* object)
{
	ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	
	if (!object->children.empty())
	{
		bool node_open = ImGui::TreeNodeEx(object->GetName(), baseFlags);
		if (ImGui::IsItemClicked())
			App->scene_intro->SetSelectedObject(object);

		if (node_open)
		{
			std::vector<GameObject*>::iterator child = object->children.begin();
			for (; child != object->children.end(); ++child)
			{
				DrawTree((*child));
			}
			ImGui::TreePop();
		}
	}
	else
	{
		ImGuiTreeNodeFlags finalFlags = baseFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		bool node_open = ImGui::TreeNodeEx(object->GetName(), finalFlags);
		if (ImGui::IsItemClicked())
			App->scene_intro->SetSelectedObject(object);
	}
}

void w_Hierarchy::CleanUp()
{

}
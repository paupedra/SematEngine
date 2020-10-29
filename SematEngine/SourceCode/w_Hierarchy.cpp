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

void w_Hierarchy::Draw()
{
	if (!active)
		return;

	if (ImGui::Begin("Hierarchy", &active))
	{
		ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		std::vector<GameObject*>::iterator item = App->scene_intro->gameObjects.begin();
		for (; item != App->scene_intro->gameObjects.end(); ++item)
		{
			bool node_open = ImGui::TreeNodeEx((*item)->GetName(), base_flags);
			
			if (ImGui::IsItemClicked())
			{
				App->scene_intro->SetSelectedObject((*item));
			}

			if (node_open)
			{
				if (!(*item)->children.empty())
				{
					std::vector<GameObject*>::iterator child = (*item)->children.begin();
					for (; child != (*item)->children.end(); ++child)
					{
						ImGui::Button((*child)->GetName());
						if(ImGui::IsItemClicked())
							App->scene_intro->SetSelectedObject((*child)); 
					}
				}
				ImGui::TreePop();
			}
		}
		ImGui::End();
	}
}

void w_Hierarchy::CleanUp()
{

}
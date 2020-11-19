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

	std::vector<GameObject*>::iterator item = App->scene_intro->rootObject->children.begin();
	for (; item != App->scene_intro->rootObject->children.end(); ++item)
		DrawTree((*item));
		
	ImGui::End();
	
}

void w_Hierarchy::DrawTree(GameObject* object)
{
	ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	
	if (object->children.empty())
		baseFlags |= ImGuiTreeNodeFlags_Leaf;

	if(ImGui::TreeNodeEx(object->GetName(), baseFlags))
	{ 
        if (ImGui::IsItemClicked())
        	App->scene_intro->SetSelectedObject(object);
        
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("Dragged GO", object, sizeof(GameObject));
			ImGui::Text("%s", object->GetName());
			draggedObject = object;

			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Dragged GO"))
			{
				//do thing with object
				LOG("Dropped %s", draggedObject->GetName());
			}

			//LOG("Dropped %s", object->GetName());
			ImGui::EndDragDropTarget();
		}

		if (!object->children.empty())
		{
			std::vector<GameObject*>::iterator child = object->children.begin();
			for (; child != object->children.end(); ++child)
			{
				DrawTree((*child));
			}
		}

		ImGui::TreePop();
	}
}

void w_Hierarchy::CleanUp()
{

}
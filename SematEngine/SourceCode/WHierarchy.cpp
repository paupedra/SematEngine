#include "Application.h"
#include "GameObject.h"
#include "Window.h"

#include "MScene.h"

#include "WHierarchy.h"

#include "Dependecies/imgui/imgui.h"

#include "Dependecies/mmgr/mmgr.h"

WHierarchy::WHierarchy(bool _active) : Window(_active)
{

}

WHierarchy::~WHierarchy()
{

}

void WHierarchy::Init()
{

}

void WHierarchy::Draw()
{
	if (!active)
		return;

	if (!ImGui::Begin("Hierarchy", &active))
	{
		ImGui::End();
		return;
	}

	ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	std::vector<GameObject*>::iterator item = App->scene->rootObject->children.begin();
	for (; item != App->scene->rootObject->children.end(); ++item)
		DrawTree((*item));

	//Right click
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
	{

		if (ImGui::IsWindowHovered())
		{
			ImGui::OpenPopup("HierarchyRightClick");
		}
	}

	if(ImGui::BeginPopup("HierarchyRightClick"))
	{
		if(ImGui::Selectable("Create Empty"))
		{
			LOG("Create Emty child here");
			if (App->scene->selectedObject == nullptr)
			{
				App->scene->rootObject->AddChild(App->scene->CreateGameObject("New Object"));
			}
			else
				App->scene->selectedObject->AddChild(App->scene->CreateGameObject("New Object"));
		}

		if (ImGui::Selectable("Delete"))
		{
			LOG("Delete selected GameObject");
			App->scene->PrepareToDestroyGameObject(App->scene->selectedObject);

			App->scene->SetSelectedObject(nullptr);
		}


		ImGui::EndPopup();
	}
		
	ImGui::End();
}

void WHierarchy::DrawTree(GameObject* object)
{
	ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	
	if (object->children.empty())
		baseFlags |= ImGuiTreeNodeFlags_Leaf;

	if(ImGui::TreeNodeEx(object->GetName(), baseFlags))
	{ 
        if (ImGui::IsItemClicked())
        	App->scene->SetSelectedObject(object);

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			if (ImGui::IsWindowHovered())
			{
				App->scene->SetSelectedObject(object);
			}
		}
        
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
				LOG("Dropped %s on %s", draggedObject->GetName(),object->GetName());
				draggedObject->Reparent(object);
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

void WHierarchy::CleanUp()
{

}
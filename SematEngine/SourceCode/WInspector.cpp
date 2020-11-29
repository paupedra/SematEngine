#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "Window.h"
#include "Resource.h"

#include "MScene.h"

#include "CTransform.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CCamera.h"

#include "RMesh.h"
#include "RMaterial.h"
#include "RTexture.h"

#include "WInspector.h"

#include "Dependecies/imgui/imgui.h"
#include "Dependecies/MathGeoLib/src/MathGeoLib.h"
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
			DrawComponent((*item));
		}
	}

	ImGui::End();
}

void WInspector::DrawComponent(Component* component)
{
	switch (component->GetType())
	{
		case ComponentType::TRANSFORM:
			DrawTransform((CTransform*)component);

			break;

		case ComponentType::MESH:
			DrawMesh((CMesh*)component);

			break;

		case ComponentType::MATERIAL:
			DrawMaterial((CMaterial*)component);
			break;

		case ComponentType::CAMERA:
			DrawCamera((CCamera*)component);
			break;
	}
}

void WInspector::DrawTransform(CTransform* component)
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue ;

		float3 position = component->GetPosition();
		float3 scale = component->GetScale();
		float3 rotation = component->GetEulerRotation();

		if (ImGui::InputFloat3("Transform", (float*)&position, "%.2f", flags)) 
		{ 
			component->SetPosition(position);
			component->UpdateLocalTransform(); 
		};
		if (ImGui::InputFloat3("Scale", (float*)&scale, "%.2f", flags)) 
		{ 
			component->SetScale(scale);
			component->UpdateLocalTransform(); 
		};
		if (ImGui::InputFloat3("Rotation", (float*)&rotation, "%.2f", flags)) 
		{ 
			component->SetEulerRotationUI(rotation);
			component->SetEulerRotation(component->GetEulerRotation()); 
		}
	}
}

void WInspector::DrawMesh(CMesh* component)
{
	if (ImGui::CollapsingHeader("Mesh"))
	{
		ImGui::Text("Path: %s", component->GetPath());
		ImGui::Text("Vertices: %d", component->GetMesh()->buffersSize[RMesh::vertex]);
		ImGui::Checkbox("Active", &component->active);
		ImGui::Checkbox("Draw Vertex Normals", &component->drawVertexNormals);
		ImGui::Checkbox("Draw AABB", &component->drawAABB);
	}
}

void WInspector::DrawMaterial(CMaterial* component)
{
	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::Text("Path: %s", component->GetPath());
		ImGui::Text("Texture height: %d", component->GetTexture()->GetHeight());
		ImGui::Text("Texture width: %d", component->GetTexture()->GetWidth());
		if (ImGui::Checkbox("DrawTexture", &component->GetTexture()->drawTexture)) {}
	}
}

void WInspector::DrawCamera(CCamera* component)
{

}

void WInspector::CleanUp()
{

}
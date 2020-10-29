#include "Application.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "I_Mesh.h"
#include "Dependecies/imgui/imgui.h"

ComponentMesh::ComponentMesh(GameObject* owner) : Component(ComponentType::MESH,owner)
{

}

ComponentMesh::ComponentMesh(GameObject* owner,char* path,Mesh* mesh = nullptr) : Component(ComponentType::MESH, owner) , mesh(mesh), path(path)
{
}

ComponentMesh::~ComponentMesh()
{

}

void ComponentMesh::Update()
{
	DrawMesh();
}

void  ComponentMesh::DrawInspector()
{
	if (ImGui::CollapsingHeader("Mesh"))
	{
		ImGui::Text("Path: %s", path);
		ImGui::Text("Vertex: %d", mesh->buffersSize[Mesh::vertex]);
	}
}

void ComponentMesh::DrawMesh()
{
	App->renderer3D->DrawMesh(mesh, owner->transform->GetTransform());
}

char* ComponentMesh::GetPath()const
{
	return path;
}
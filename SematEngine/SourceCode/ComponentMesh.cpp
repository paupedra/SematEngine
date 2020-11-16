#include "Application.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentTexture.h"
#include "I_Texture.h"
#include "I_Mesh.h"
#include "Dependecies/imgui/imgui.h"
#include "Dependecies/mmgr/mmgr.h"

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

void ComponentMesh::CleanUp()
{
	//delete path; //What is this
	delete mesh;
}

void  ComponentMesh::DrawInspector()
{
	if (ImGui::CollapsingHeader("Mesh"))
	{
		ImGui::Text("Path: %s", path);
		ImGui::Text("Vertices: %d", mesh->buffersSize[Mesh::vertex]);
		ImGui::Checkbox("Active", &this->active);
		ImGui::Checkbox("Draw Vertex Normals", &drawVertexNormals);
	}
}

void ComponentMesh::DrawMesh()
{
	if (!this->active)
		return;

	if (owner->GetComponent<ComponentTexture>() != nullptr)
	{
		if (owner->GetComponent<ComponentTexture>()->IsEnabled()) //
		{
			App->renderer3D->DrawMesh(mesh, owner->transform->GetTransform(), owner->GetComponent<ComponentTexture>()->GetTexture()->id ,drawVertexNormals);
			return;
		}
	}

	App->renderer3D->DrawMesh(mesh, owner->transform->GetTransform(),0, drawVertexNormals);
}

char* ComponentMesh::GetPath()const
{
	return path;
}
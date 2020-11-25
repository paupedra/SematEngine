#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "Resource.h"

#include "MRenderer3D.h"

#include "CMesh.h"
#include "CTransform.h"
#include "CMaterial.h"

#include "ITexture.h"
#include "IMesh.h"

#include "RMesh.h"

#include "Dependecies/imgui/imgui.h"

#include "Dependecies/mmgr/mmgr.h"

CMesh::CMesh(GameObject* owner) : Component(ComponentType::MESH,owner)
{

}

CMesh::CMesh(GameObject* owner,char* path,RMesh* mesh = nullptr) : Component(ComponentType::MESH, owner) , mesh(mesh), path(path)
{
}

CMesh::~CMesh()
{

}

void CMesh::Update()
{
	DrawMesh();
}

void CMesh::CleanUp()
{
	delete mesh;
}

void  CMesh::DrawInspector()
{
	if (ImGui::CollapsingHeader("Mesh"))
	{
		ImGui::Text("Path: %s", path);
		ImGui::Text("Vertices: %d", mesh->buffersSize[RMesh::vertex]);
		ImGui::Checkbox("Active", &this->active);
		ImGui::Checkbox("Draw Vertex Normals", &drawVertexNormals);
	}
}

void CMesh::DrawMesh()
{
	if (!this->active)
		return;

	//LOG("Drawing %s mesh", owner->GetName());

	if (owner->GetComponent<CMaterial>() != nullptr)
	{
		if (owner->GetComponent<CMaterial>()->IsEnabled()) //
		{
			App->renderer3D->DrawMesh(mesh, owner->transform->GetGlobalTransform(), owner->GetComponent<CMaterial>()->GetTexture() ,drawVertexNormals);
			return;
		}
	}

	App->renderer3D->DrawMesh(mesh, owner->transform->GetGlobalTransform(),nullptr, drawVertexNormals);
}

char* CMesh::GetPath()const
{
	return path;
}
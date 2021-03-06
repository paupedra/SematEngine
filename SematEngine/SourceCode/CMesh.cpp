#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "Resource.h"
#include "Config.h"

#include "MRenderer3D.h"
#include "MResourceManager.h"

#include "CMesh.h"
#include "CTransform.h"
#include "CMaterial.h"

#include "ITexture.h"
#include "IMesh.h"

#include "RMesh.h"

#include "Dependecies/imgui/imgui.h"

//#include "Dependecies/mmgr/mmgr.h"

CMesh::CMesh(GameObject* owner) : Component(ComponentType::MESH,owner)
{

}

CMesh::CMesh(GameObject* owner,char* path,RMesh* mesh = nullptr) : Component(ComponentType::MESH, owner) , mesh(mesh), path(path)
{
}

CMesh::~CMesh()
{

}

void CMesh::Update(float dt)
{
	DrawMesh();
}

void CMesh::CleanUp()
{
	//dereference RMesh
	if(mesh != nullptr)
		App->resourceManager->DereferenceResource(mesh->resourceData.UID);
}

void CMesh::Serialize(JsonNode* node)
{
	node->AddNumber("Mesh UID", mesh->GetUID());
}

void CMesh::Load(JsonNode* node)
{
	if (node->GetNumber("Mesh UID") != 0)
	{
		App->resourceManager->LoadMesh(node->GetNumber("Mesh UID"));
		mesh = (RMesh*)App->resourceManager->RequestResource(node->GetNumber("Mesh UID"));
	}

}

void CMesh::DrawMesh()
{
	if (!this->active)
		return;

	CMaterial* material = owner->GetComponent<CMaterial>();

	if (material != nullptr)
	{
		if (material->IsEnabled())
		{
			App->renderer3D->DrawMesh(mesh, owner->transform->GetGlobalTransform(), material ,drawVertexNormals,drawAABB,owner);
			return;
		}
	}

	App->renderer3D->DrawMesh(mesh, owner->transform->GetGlobalTransform(),nullptr, drawVertexNormals,drawAABB,owner);

	//App->renderer3D->DrawBoundingBox(mesh,owner->transform->GetGlobalTransform());
}

void CMesh::OnSave(JsonNode* node)
{
	//save id
}

void CMesh::SetMesh(RMesh* _mesh)
{
	mesh = _mesh;
	owner->UpdateBoundingBoxes();
}

char* CMesh::GetPath()const
{
	return path;
}

RMesh* CMesh::GetMesh()const
{
	return mesh;
}
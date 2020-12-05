#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "Resource.h"
#include "Config.h"

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

void CMesh::DrawMesh()
{
	if (!this->active)
		return;

	const CMaterial* material = owner->GetComponent<CMaterial>();

	if (material != nullptr)
	{
		if (material->IsEnabled())
		{
			App->renderer3D->DrawMesh(mesh, owner->transform->GetGlobalTransform(), material->GetMaterial() ,drawVertexNormals,drawAABB,owner);
			return;
		}
	}

	App->renderer3D->DrawMesh(mesh, owner->transform->GetGlobalTransform(),nullptr, drawVertexNormals,drawAABB,owner);

	//App->renderer3D->DrawBoundingBox(mesh,owner->transform->GetGlobalTransform());
}

void CMesh::OnSave(ConfigNode* node)
{
	//save id
}

char* CMesh::GetPath()const
{
	return path;
}

RMesh* CMesh::GetMesh()const
{
	return mesh;
}
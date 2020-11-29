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

	//LOG("Drawing %s mesh", owner->GetName());

	if (owner->GetComponent<CMaterial>() != nullptr)
	{
		if (owner->GetComponent<CMaterial>()->IsEnabled()) //
		{
			App->renderer3D->DrawMesh(mesh, owner->transform->GetGlobalTransform(), owner->GetComponent<CMaterial>()->GetTexture() ,drawVertexNormals,drawAABB);
			return;
		}
	}

	App->renderer3D->DrawMesh(mesh, owner->transform->GetGlobalTransform(),nullptr, drawVertexNormals,drawAABB);

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
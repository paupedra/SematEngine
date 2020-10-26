#include "Application.h"
#include "I_Mesh.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

ComponentMesh::ComponentMesh()
{

}

ComponentMesh::~ComponentMesh()
{

}

void ComponentMesh::Update()
{

}

void ComponentMesh::DrawMesh()
{
	App->renderer3D->DrawMesh(&mesh, owner->transform->GetTransform());
}
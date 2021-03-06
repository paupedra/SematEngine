#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "Resource.h"
#include "Config.h"

#include "MRenderer3D.h"
#include "MResourceManager.h"

#include "CTransform.h"
#include "CMaterial.h"

#include "ITexture.h"

#include "RMaterial.h"
#include "RTexture.h"

#include "Dependecies/imgui/imgui.h"
//#include "Dependecies/mmgr/mmgr.h"

CMaterial::CMaterial(GameObject* owner) : Component(ComponentType::MATERIAL, owner)
{
	material = new RMaterial();
	//material->GetTexture()->usesTexture = false;
}

CMaterial::CMaterial(GameObject* owner, const char* path, RMaterial* material,RTexture* texture) : Component(ComponentType::MATERIAL, owner), material(material), path(path) 
{
	if(material == nullptr)
		this->material = new RMaterial();
	else
		material->SetTexture(texture);
}

CMaterial::~CMaterial()
{

}

void CMaterial::Update(float dt)
{
	
}

void CMaterial::CleanUp()
{
	if(material != nullptr)
		App->resourceManager->DereferenceResource(material->GetUID());

}

void CMaterial::OnSave(JsonNode* node)
{
	//color

	//id textura?
}

void CMaterial::Serialize(JsonNode* node)
{
	node->AddNumber("Material UID", material->GetUID());

}

void CMaterial::Load(JsonNode* node)
{
	if (node->GetNumber("Material UID") != 0)
	{
		material = App->resourceManager->LoadMaterial(node->GetNumber("Material UID"));
	}
}

void CMaterial::SetTexture(RTexture* texture)
{
	material->SetTexture(texture);
	if(texture != nullptr)
		material->GetTexture()->usesTexture = true;
}

void CMaterial::SetMaterial(RMaterial* material)
{
	this->material = material;
}

const char* CMaterial::GetPath()const
{
	return path;
}

RMaterial* CMaterial::GetMaterial() const
{
	return material;
}

RTexture* CMaterial::GetTexture()const
{
	return material->GetTexture();
}

bool CMaterial::IsEnabled() const
{
	return drawTexture;
}

void CMaterial::SwitchEnabledTexture()
{
	active = !active;
}
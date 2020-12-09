#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "Resource.h"
#include "Config.h"

#include "MRenderer3D.h"

#include "CTransform.h"
#include "CMaterial.h"

#include "ITexture.h"

#include "RMaterial.h"
#include "RTexture.h"

#include "Dependecies/imgui/imgui.h"
#include "Dependecies/mmgr/mmgr.h"

CMaterial::CMaterial(GameObject* owner) : Component(ComponentType::MATERIAL, owner)
{
	material = new RMaterial();
	texture = new RTexture();
	texture->usesTexture = false;
}

CMaterial::CMaterial(GameObject* owner, const char* path, RMaterial* material,RTexture* texture) : Component(ComponentType::MATERIAL, owner), material(material), path(path) , texture(texture)
{
	if(material == nullptr)
		this->material = new RMaterial();
}

CMaterial::~CMaterial()
{

}

void CMaterial::Update()
{
	
}

void CMaterial::CleanUp()
{
	material->CleanUp();

	delete material;
}

void CMaterial::OnSave(ConfigNode* node)
{
	//color

	//id textura?
}

void CMaterial::SetTexture(RTexture* texture)
{
	this->texture = texture;
	this->texture->usesTexture = true;
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
	return texture;
}

bool CMaterial::IsEnabled() const
{
	return drawTexture;
}

void CMaterial::SwitchEnabledTexture()
{
	active = !active;
}
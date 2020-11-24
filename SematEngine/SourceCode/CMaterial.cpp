#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "Resource.h"

#include "MRenderer3D.h"

#include "CTransform.h"
#include "CMaterial.h"

#include "ITexture.h"

#include "RMaterial.h"

#include "Dependecies/imgui/imgui.h"
#include "Dependecies/mmgr/mmgr.h"

CMaterial::CMaterial(GameObject* owner) : Component(ComponentType::TEXTURE, owner)
{

}

CMaterial::CMaterial(GameObject* owner, const char* path, RMaterial* material = nullptr) : Component(ComponentType::TEXTURE, owner), material(material), path(path)
{

}

CMaterial::~CMaterial()
{

}

void CMaterial::Update()
{
	
}

void CMaterial::CleanUp()
{
	delete material;
	//delete path; //What is this
}

void CMaterial::DrawInspector()
{
	if (ImGui::CollapsingHeader("Texture"))
	{
		ImGui::Text("Path: %s", path);
		ImGui::Text("Texture height: %d", material->GetHeight());
		ImGui::Text("Texture width: %d", material->GetWidth());
		if (ImGui::Checkbox("DrawTexture", &drawTexture)) {}
	}
}

const char* CMaterial::GetPath()const
{
	return path;
}

RMaterial* CMaterial::GetTexture() const
{
	return material;
}

bool CMaterial::IsEnabled() const
{
	return drawTexture;
}

void CMaterial::SwitchEnabledTexture()
{
	active = !active;
}
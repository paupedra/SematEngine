#include "Application.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "I_Texture.h"
#include "ComponentTexture.h"
#include "Dependecies/imgui/imgui.h"
#include "Dependecies/mmgr/mmgr.h"

ComponentTexture::ComponentTexture(GameObject* owner) : Component(ComponentType::TEXTURE, owner)
{

}

ComponentTexture::ComponentTexture(GameObject* owner, const char* path, Texture* texture = nullptr) : Component(ComponentType::TEXTURE, owner), texture(texture), path(path)
{

}

ComponentTexture::~ComponentTexture()
{

}

void ComponentTexture::Update()
{
	
}

void ComponentTexture::CleanUp()
{
	delete texture;
	//delete path; //What is this
}

void ComponentTexture::DrawInspector()
{
	if (ImGui::CollapsingHeader("Texture"))
	{
		ImGui::Text("Path: %s", path);
		ImGui::Text("Texture height: %d", texture->height);
		ImGui::Text("Texture width: %d", texture->width);
		if (ImGui::Checkbox("DrawTexture", &drawTexture)) {}
	}
}

const char* ComponentTexture::GetPath()const
{
	return path;
}

Texture* ComponentTexture::GetTexture() const
{
	return texture;
}

bool ComponentTexture::IsEnabled() const
{
	return drawTexture;
}

void ComponentTexture::SwitchEnabledTexture()
{
	active = !active;
}
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "Window.h"
#include "Resource.h"
#include "Color.h"

#include "MScene.h"
#include "MCamera3D.h"
#include "MEditor.h"

#include "CTransform.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CCamera.h"
#include "CAnimator.h"

#include "RMesh.h"
#include "RMaterial.h"
#include "RTexture.h"
#include "RAnimation.h"

#include "WInspector.h"

#include "Dependecies/imgui/imgui.h"
#include "Dependecies/MathGeoLib/include/MathGeoLib.h"
//#include "Dependecies/mmgr/mmgr.h"

WInspector::WInspector(bool _active) : Window(_active)
{

}

WInspector::~WInspector()
{

}

void WInspector::Init()
{

}

void WInspector::Draw()
{
	if (!active)
		return;

	if (!ImGui::Begin("Inspector", &active))
	{
		ImGui::End();
		return;
	}

	if (App->scene->selectedObject != nullptr)
	{
		std::vector<Component*> vector = App->scene->selectedObject->GetComponents();
		std::vector<Component*>::iterator item = vector.begin();
		for (; item != vector.end(); ++item)
		{
			DrawComponent((*item));
		}

		ImGui::Separator();

		if (ImGui::Button("Add new component"))
		{
			ImGui::OpenPopup("Add Component");
		}

		if (ImGui::BeginPopup("Add Component"))
		{
			if (ImGui::Selectable("Mesh"))
			{
				App->scene->selectedObject->AddComponent(ComponentType::MESH);
			}
			if (ImGui::Selectable("Material"))
			{
				App->scene->selectedObject->AddComponent(ComponentType::MATERIAL);
			}
			if (ImGui::Selectable("Camera"))
			{
				App->scene->selectedObject->AddComponent(ComponentType::CAMERA);
			}
			ImGui::EndPopup();
		}
	}
	if (ImGui::IsWindowHovered())
		App->editor->mouseHovered = true;
	ImGui::End();
}

void WInspector::DrawComponent(Component* component)
{
	switch (component->GetType())
	{
		case ComponentType::TRANSFORM:
			DrawTransform((CTransform*)component);
			break;

		case ComponentType::MESH:
			DrawMesh((CMesh*)component);
			break;

		case ComponentType::MATERIAL:
			DrawMaterial((CMaterial*)component);
			break;

		case ComponentType::CAMERA:
			DrawCamera((CCamera*)component);
			break;

		case ComponentType::ANIMATOR:
			DrawAnimator((CAnimator*)component);
			break;
	}
}

void WInspector::DrawTransform(CTransform* component)
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue ;

		float3 position = component->GetPosition();
		float3 scale = component->GetScale();
		float3 rotation = component->GetEulerRotation();

		if (ImGui::DragFloat3("Position", (float*)&position, 0.25)) 
		{ 
			component->SetPosition(position);
			component->UpdateLocalTransform(); 
		};
		if (ImGui::DragFloat3("Scale", (float*)&scale, 0.25,0.1f,100.f))
		{ 
			component->SetScale(scale);
			component->UpdateLocalTransform(); 
		};
		if (ImGui::DragFloat3("Rotation", (float*)&rotation, 1))
		{ 
			//component->SetEulerRotationUI(rotation);
			component->SetEulerRotation(rotation);
		}
	}
}

void WInspector::DrawMesh(CMesh* component)
{
	if (ImGui::CollapsingHeader("Mesh"))
	{
		ImGui::Text("Path: %s", component->GetPath());
		ImGui::Checkbox("Active", &component->active);
		ImGui::Checkbox("Draw Vertex Normals", &component->drawVertexNormals);
		ImGui::Checkbox("Draw AABB", &component->drawAABB);
		ImGui::Separator();
		if (component->GetMesh() != nullptr)
		{
			ImGui::Text("Vertices: %d", component->GetMesh()->buffersSize[RMesh::vertex]);
		}
	}
}

void WInspector::DrawMaterial(CMaterial* component)
{
	if (ImGui::CollapsingHeader("Material"))
	{
		if (component->GetTexture() != nullptr)
		{
			ImGui::Text("Path: %s", component->GetPath());
			ImGui::Text("Texture height: %d", component->GetTexture()->GetHeight());
			ImGui::Text("Texture width: %d", component->GetTexture()->GetWidth());
			if (ImGui::Checkbox("DrawTexture", &component->drawTexture)) {}
		}

		Color color = component->GetMaterial()->GetColor();
		if (ImGui::DragFloat3("Color", (float*)&color, 0.1f)) { component->GetMaterial()->SetColor(color); }


	}
}

void WInspector::DrawCamera(CCamera* component)
{
	ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly;
	
	if (ImGui::CollapsingHeader("Camera"))
	{
		float3 pos = component->GetPos();
		if (ImGui::DragFloat3("Camera Position", (float*)&pos,0.5))
		{
			component->Setposition(pos);
		}

		float nearPlane = component->GetNearPlaneDistance();
		if (ImGui::DragFloat("Near Plane Distance", &nearPlane, 0.5f,0.f))
		{
			component->SetNearPlane(nearPlane);
		}

		float farPlane = component->GetFarPlaneDistance();
		if (ImGui::DragFloat("Far Plane Distance", &farPlane, 0.5f,0.1f))
		{
			component->SetFarPlane(farPlane);
		}

		float verticalFov = component->GetVerticalFov();
		if (ImGui::DragFloat("Vertical Fov", &verticalFov,0.1f, 30.f, 120.f))
		{
			component->SetVerticalFov(verticalFov);
		}

		ImGui::Checkbox("Culling", &component->cull);
		if(ImGui::Button("Set As Current Camera"))
		{
			App->camera->SetCurrentCamera(component);
		}

		if (ImGui::Button("Set As Culling Camera"))
		{
			App->camera->SetCullingCamera(component);
		}
	}
}

void WInspector::DrawAnimator(CAnimator* animator)
{
	if (ImGui::CollapsingHeader("Animator"))
	{
		ImGui::Text("Amount of animations: %d", animator->GetAnimationsSize());
		ImGui::Text("Current animation: %s", animator->GetAnimationName());
		ImGui::Separator();

		float playbackSpeed = animator->GetPlaybackSpeed();
		if (ImGui::DragFloat("Playback Speed", &playbackSpeed, 0.1,0.1,10))
		{
			animator->SetPlaybackSpeed(playbackSpeed);
		}

		if (animator->GetCurrentAnimation() != nullptr)
		{
			if (ImGui::Button("Play"))
				animator->Play();

			ImGui::SameLine();

			if (ImGui::Button("Pause"))
				animator->Pause();

			ImGui::SameLine();

			if (ImGui::Button("Stop"))
				animator->Stop();

			ImGui::Text("Ticks: %f ticks", animator->GetAnimationTicks());
			ImGui::Text("Time: %f seconds", animator->GetAnimationTime());
			ImGui::Text("Speed: %f ticks/s", animator->GetAnimationSpeed());
			ImGui::Text("Duration in ticks: %f ticks", animator->GetAnimationDuration());
			ImGui::Text("Duration in seconds: %f seconds", animator->GetDurationInSeconds());

			bool drawBones = animator->GetDrawBones();
			if (ImGui::Checkbox("Draw Bones", &drawBones)) { animator->SwitchDrawBones(); }
		
			ImGui::Separator();

			ImGui::Text("Chops:");
			//for (std::vector<AnimationChop>::iterator it = animator->GetChops(); it != animator->GetChops().end(); it++)
			std::vector<AnimationChop>* chops = animator->GetChops();
			for(std::vector<AnimationChop>::iterator it = chops->begin(); it != chops->end(); it++)
			{
				float _startKey = it->startKey;
				float _endKey = it->endKey;
				float _speed = it->speed;
				if (ImGui::SliderFloat("Start Key: ", &_startKey, 0, it->owner->duration))
				{
					it->SetStartKey(_startKey);
				}
				if (ImGui::SliderFloat("End Key: ", &_endKey, 0, it->owner->duration))
				{
					it->SetEndKey(_endKey);
				}
				if (ImGui::SliderFloat("Speed: ", &_speed, 1, 100))
				{
					it->SetSpeed(_speed);
				}
				ImGui::Separator();
			}

			if (ImGui::Button("Add Chop"))
			{
				ImGui::OpenPopup("Add Chop Popup");
				speed = animator->GetAnimationSpeed();
			}

			if (ImGui::BeginPopup("Add Chop Popup"))
			{
				ImGui::InputFloat("Start Key", &startKey);
				ImGui::InputFloat("End Key", &endKey);
				ImGui::InputFloat("Speed", &speed);

				if (ImGui::Button("Add"))
				{
					animator->AddChop(startKey, endKey, speed);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}

		ImGui::Text("Animations:");
		for (uint i = 0; i < animator->GetAnimations().size(); i++)
		{
			if(ImGui::Button(animator->GetAnimation(i)->name.c_str()))
			{
				animator->SetCurrentAnimation(animator->GetAnimation(i));
			}
		}
	}
}

void WInspector::CleanUp()
{

}
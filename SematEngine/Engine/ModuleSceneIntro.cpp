#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

#include "Dependecies/imgui/imgui.h"


ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	

	

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

void ModuleSceneIntro::HandleDebugInput()
{
	
}

void ModuleSceneIntro::DebugSpawnPrimitive(Primitive * p)
{

}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	ImGui::ShowDemoWindow();

	//bool my_tool_active = true;
	//ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
	//if (ImGui::BeginMenuBar())
	//{
	//	if (ImGui::BeginMenu("File"))
	//	{
	//		if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
	//		if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
	//		if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
	//		ImGui::EndMenu();
	//	}
	//	ImGui::EndMenuBar();
	//}

	
	//ImGui::EndFrame();

	//ImGui::End();
	


	Plane p(vec3(0, 1, 0));
	p.axis = true;
	p.Render();

	if (App->debug == true)
		HandleDebugInput();

	for (uint n = 0; n < primitives.Count(); n++)
		primitives[n]->Update();

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	for (uint n = 0; n < primitives.Count(); n++)
	{
		primitives[n]->Render();
	}

	

	return UPDATE_CONTINUE;
}
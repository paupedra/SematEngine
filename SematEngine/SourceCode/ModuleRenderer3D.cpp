#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"

#include "Dependecies/Glew/include/glew.h"
#include "Dependecies/SDL/include/SDL_opengl.h" 
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Editor.h"
#include "Dependecies/Brofiler/Brofiler.h"

#include <Windows.h>

#include "Dependecies/imgui/imgui.h"
#include "Dependecies/imgui/imgui_internal.h"
#include "Dependecies/imgui/imgui_impl_sdl.h"
#include "Dependecies/imgui/imgui_impl_opengl3.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Dependecies/Glew/libx86/glew32.lib")
//#pragma comment (lib, "Dependecies/Glew/libx86/glew32s.lib")

#include "I_Mesh.h"
#include <vector>

ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled), context()
{
	cullFace = true;
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_LINE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);

	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	if (glewInit() != GLEW_OK) {
		LOG("ERROR ON GLEWINIT");
		ret = false;
	}

	Importer::MeshImp::Import("Assets/Mesh/warrior/warrior.FBX");

	//Importer::MeshImp::Import("Assets/Mesh/BakerHouse/BakerHouse.FBX",&currentId);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);

	glLoadMatrixf(App->camera->GetRawViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	//File dropping
	FileDropCheck();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{

	std::vector<Mesh*>::iterator item = meshes.begin();

	for (; item != meshes.end() ;++item)
	{
		DrawMesh( (*item) );
	}

	DrawCube();

	//DrawMesh(meshes[0]);
	

	BROFILER_CATEGORY("Draw imgui", Profiler::Color::Azure)
	App->editor->Draw();

	BROFILER_CATEGORY("SwapWindow", Profiler::Color::Azure)
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::DrawMesh(Mesh* mesh)
{
	glEnableClientState(GL_VERTEX_ARRAY);



	glBindBuffer(GL_ARRAY_BUFFER, mesh->buffersId[Mesh::vertex]);

	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->buffersId[Mesh::index]);
	
	glDrawElements(GL_TRIANGLES, mesh->buffersSize[Mesh::index], GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleRenderer3D::GenerateBuffers(Mesh* newMesh)
{
	//Vertex buffer
	glGenBuffers(1, (GLuint*)&(newMesh->buffersId[Mesh::vertex]));
	glBindBuffer(GL_ARRAY_BUFFER, newMesh->buffersId[Mesh::vertex]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * newMesh->buffersSize[Mesh::vertex] * 3, &newMesh->vertices[0], GL_STATIC_DRAW);


	if (newMesh->indices != nullptr)
	{
		//Index buffer
		glGenBuffers(1, (GLuint*)&(newMesh->buffersId[Mesh::index]));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh->buffersId[Mesh::index]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * newMesh->buffersSize[Mesh::index], &newMesh->indices[0], GL_STATIC_DRAW);
	}
}

void ModuleRenderer3D::FileDropCheck()
{
	//SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
	
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case (SDL_DROPFILE):

			Importer::MeshImp::Import(event.drop.file);

			LOG("Mesh imported");

			break;
		}
	}
}

void ModuleRenderer3D::SwitchCullFace()
{
	if (cullFace)
	
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}

void ModuleRenderer3D::DrawCube()
{
	uint my_id = 0;

	glEnableClientState(GL_VERTEX_ARRAY);

	float cube_vertex_coords[108] =
	{
		0.0f, 0.0f, 0.0f,        //ABC
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 0.0f,        //ACD
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		1.0f, 0.0f, 0.0f,        //BFC
		1.0f, 0.0f, -1.0f,
		1.0f, 1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,        //CFH
		1.0f, 0.0f, -1.0f,
		1.0f, 1.0f, -1.0f,

		1.0f, 1.0f, 0.0f,        //CHG
		1.0f, 1.0f, -1.0f,
		0.0f, 1.0f, -1.0f,

		1.0f, 1.0f, 0.0f,        //CGD
		0.0f, 1.0f, -1.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, 1.0f, 0.0f,        //DGE
		0.0f, 1.0f, -1.0f,
		0.0f, 0.0f, -1.0f,

		0.0f, 1.0f, 0.0f,        //DEA
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.0f,        //AEB
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,        //BEF
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, -1.0f,

		0.0f, 0.0f, -1.0f,        //EHF
		1.0f, 1.0f, -1.0f,
		1.0f, 0.0f, -1.0f,

		0.0f, 0.0f, -1.0f,        //EGH
		0.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f
	};

	glGenBuffers(1, (GLuint*)&my_id);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 3, cube_vertex_coords, GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisableClientState(GL_VERTEX_ARRAY);
}
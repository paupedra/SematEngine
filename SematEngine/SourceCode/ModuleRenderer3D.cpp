#include "I_Mesh.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "I_Texture.h"

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

//#include "Light.h"
#include "ModuleRenderer3D.h"

#include <vector>

#include "Dependecies/mmgr/mmgr.h"

ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled), context()
{
	glCullFace = true;
	glColorMaterial = true;
	glLighting = false;
	glDepthTest = true;
	glTexture2d = true;
	wireframeMode = false;
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
		//Init Glew
		if (glewInit() != GLEW_OK) {
			LOG("ERROR ON GLEWINIT");
			ret = false;
		}
		else
			LOG("Glew initialized succesfully!");

		LOG("Vendor: %s", glGetString(GL_VENDOR));
		LOG("Renderer: %s", glGetString(GL_RENDERER));
		LOG("OpenGL version supported %s", glGetString(GL_VERSION));
		LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

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
		//glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
		lights[0].Active(true);
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	CreateChekerTexture();
	//InitCube();

	Importer::TextureImp::InitDevil();

	Texture tmp = Importer::TextureImp::Import("Assets/Mesh/BakerHouse/Baker_house.png");
	houseId = tmp.id;

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
	ImGui::ShowDemoWindow();

	//DrawCube();

	BROFILER_CATEGORY("Draw imgui", Profiler::Color::AliceBlue)
	App->editor->Draw();

	BROFILER_CATEGORY("SwapWindow", Profiler::Color::GoldenRod)
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

void ModuleRenderer3D::DrawMesh(Mesh* mesh, float4x4 transform)
{
	wireframeMode == false ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) : (glPolygonMode(GL_FRONT_AND_BACK, GL_LINE), glColor4f(255,255, 0, 255));
	
	glLineWidth(2);

	glPushMatrix();
	glMultMatrixf((float*)&transform);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//Texture
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	//Pass TextureID
	glBindTexture(GL_TEXTURE_2D, houseId);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->buffersId[Mesh::texture]);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->buffersId[Mesh::vertex]);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->buffersId[Mesh::index]);
	glDrawElements(GL_TRIANGLES, mesh->buffersSize[Mesh::index], GL_UNSIGNED_INT, NULL);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
}

void ModuleRenderer3D::GenerateBuffers(Mesh* newMesh)
{
	//Vertex buffer
	glGenBuffers(1, (GLuint*)&(newMesh->buffersId[Mesh::vertex]));
	glBindBuffer(GL_ARRAY_BUFFER, newMesh->buffersId[Mesh::vertex]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * newMesh->buffersSize[Mesh::vertex] * 3, newMesh->vertices, GL_STATIC_DRAW);


	if (newMesh->indices != nullptr)
	{
		//Index buffer
		glGenBuffers(1, (GLuint*)&(newMesh->buffersId[Mesh::index]));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh->buffersId[Mesh::index]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * newMesh->buffersSize[Mesh::index], newMesh->indices, GL_STATIC_DRAW);
	}

	if (newMesh->normals != nullptr)
	{
		//Normals buffer
		glGenBuffers(1, (GLuint*)&(newMesh->buffersId[Mesh::normal]));
		glBindBuffer(GL_ARRAY_BUFFER, newMesh->buffersId[Mesh::normal]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * newMesh->buffersSize[Mesh::normal] * 3, newMesh->normals, GL_STATIC_DRAW);
	}

	if (newMesh->textureCoords != nullptr)
	{
		glGenBuffers(1, (GLuint*)&newMesh->buffersId[Mesh::texture]);
		glBindBuffer(GL_ARRAY_BUFFER, newMesh->buffersId[Mesh::texture]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * newMesh->buffersSize[Mesh::texture] * 2, newMesh->textureCoords, GL_STATIC_DRAW);
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

			LOG("File was dropped");
			Importer::MeshImp::Import(event.drop.file);

			break;
		}
	}
}

void ModuleRenderer3D::CreateChekerTexture()
{
	GLubyte checkerImage[64][64][4];
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;

			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &checkersId);
	glBindTexture(GL_TEXTURE_2D, checkersId);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ModuleRenderer3D::SwitchCullFace()
{
	glIsEnabled(GL_CULL_FACE) == false? glEnable(GL_CULL_FACE): glDisable(GL_CULL_FACE);
}

void ModuleRenderer3D::SwitchDepthTest()
{
	if (glDepthTest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void ModuleRenderer3D::SwitchLighting()
{
	if (glLighting)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
}

void ModuleRenderer3D::SwitchTexture2d()
{
	if (glTexture2d)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
}

void ModuleRenderer3D::SwitchColorMaterial()
{
	if (glColorMaterial)
		glEnable(GL_COLOR_MATERIAL);
	else
		glDisable(GL_COLOR_MATERIAL);
}
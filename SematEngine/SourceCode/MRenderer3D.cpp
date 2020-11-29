#include <vector>
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "Resource.h"

#include "MWindow.h"
#include "MCamera3D.h"
#include "MScene.h"
#include "MEditor.h"
#include "MRenderer3D.h"

#include "IMesh.h"
#include "ITexture.h"

#include "CMaterial.h"

#include "RMesh.h"
#include "RMaterial.h"

#include "Dependecies/Glew/include/glew.h"
#include "Dependecies/SDL/include/SDL_opengl.h" 
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Dependecies/Brofiler/Brofiler.h"

#include <Windows.h>

#include "Dependecies/imgui/imgui.h"
#include "Dependecies/imgui/imgui_internal.h"
#include "Dependecies/imgui/imgui_impl_sdl.h"
#include "Dependecies/imgui/imgui_impl_opengl3.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Dependecies/Glew/libx86/glew32.lib")

#include "Dependecies/mmgr/mmgr.h"

MRenderer3D::MRenderer3D(bool start_enabled) : Module(start_enabled), context()
{
	glCullFace = true;
	glColorMaterial = true;
	glLighting = false;
	glDepthTest = true;
	glTexture2d = true;
	wireframeMode = false;
	checkersId = 0;
}

// Destructor
MRenderer3D::~MRenderer3D()
{}

// Called before render is available
bool MRenderer3D::Init()
{
	LOG("(INIT) Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("(ERROR) OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		//Init Glew
		if (glewInit() != GLEW_OK) {
			LOG("(ERROR) ERROR ON GLEWINIT");
			ret = false;
		}
		else
			LOG("(INIT) Glew initialized succesfully!");

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
			LOG("(ERROR) Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("(ERROR) Error initializing OpenGL! %s\n", gluErrorString(error));
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
			LOG("(ERROR) Error initializing OpenGL! %s\n", gluErrorString(error));
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

		//glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glAlphaFunc(GL_GREATER, 0.3);
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	CreateChekerTexture();
	Importer::TextureImp::InitDevil();

	return ret;
}

// PreUpdate: clear buffer
update_status MRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);

	glLoadMatrixf(App->camera->GetRawViewMatrix());
		
	// light 0 on cam pos
	lights[0].SetPos(App->camera->position.x, App->camera->position.y, App->camera->position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status MRenderer3D::PostUpdate(float dt)
{
	//ImGui::ShowDemoWindow();

	BROFILER_CATEGORY("Draw imgui", Profiler::Color::AliceBlue)
	App->editor->Draw();

	BROFILER_CATEGORY("SwapWindow", Profiler::Color::GoldenRod)
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool MRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);
	return true;
}

void MRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	projectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&projectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void MRenderer3D::DrawMesh(RMesh* mesh, float4x4 transform, RMaterial* material,bool drawVertexNormals, bool drawBoundingBox)
{
	wireframeMode == false ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) : (glPolygonMode(GL_FRONT_AND_BACK, GL_LINE), glColor4f(255,255, 0, 255));
	
	glPushMatrix();
	glMultMatrixf((float*)&transform.Transposed());

	glLineWidth(2);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//Pass TextureID
	if (!wireframeMode)
	{
		if (material != nullptr)
		{
			if (material->GetId() == 0)
				glBindTexture(GL_TEXTURE_2D, checkersId);
			else
				glBindTexture(GL_TEXTURE_2D, material->GetId());
		}
		else
		{
			glColor4f(255, 100, 100, 255);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, mesh->buffersId[RMesh::texture]);
	glTexCoordPointer(2, GL_FLOAT, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->buffersId[RMesh::normal]);
	glNormalPointer(GL_FLOAT, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->buffersId[RMesh::vertex]);
	glVertexPointer(3, GL_FLOAT, 0, nullptr);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->buffersId[RMesh::index]);
	glDrawElements(GL_TRIANGLES, mesh->buffersSize[RMesh::index], GL_UNSIGNED_INT, nullptr);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		LOG("(ERROR) Problem drawing mesh: %s\n", gluErrorString(error));
	}

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (drawVertexNormals)
		DrawVertexNormals(mesh,transform);
	if (drawBoundingBox)
		DrawBoundingBox(mesh);
}

void MRenderer3D::DrawVertexNormals(RMesh* mesh,float4x4 transform)
{
	//Draw Normals
	glBegin(GL_LINES);

	glPushMatrix();
	glMultMatrixf((float*)&transform.Transposed());

	uint loops = mesh->buffersSize[RMesh::vertex];
	for (uint i = 0; i < loops; i += 3)
	{
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glVertex3f(mesh->vertices[i], mesh->vertices[i + 1], mesh->vertices[i + 2]);
		glVertex3f(mesh->vertices[i] + mesh->normals[i], mesh->vertices[i + 1] + mesh->normals[i + 1], mesh->vertices[i + 2] + mesh->normals[i + 2]);
	}

	glPopMatrix();
	glEnd();
}

void MRenderer3D::GenerateBuffers(RMesh* newMesh)
{
	//Vertex buffer
	glGenBuffers(1, (GLuint*)&(newMesh->buffersId[RMesh::vertex]));
	glBindBuffer(GL_ARRAY_BUFFER, newMesh->buffersId[RMesh::vertex]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * newMesh->buffersSize[RMesh::vertex] * 3, newMesh->vertices, GL_STATIC_DRAW);


	if (newMesh->indices != nullptr)
	{
		//Index buffer
		glGenBuffers(1, (GLuint*)&(newMesh->buffersId[RMesh::index]));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh->buffersId[RMesh::index]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * newMesh->buffersSize[RMesh::index], newMesh->indices, GL_STATIC_DRAW);
	}

	if (newMesh->normals != nullptr)
	{
		//Normals buffer
		glGenBuffers(1, (GLuint*)&(newMesh->buffersId[RMesh::normal]));
		glBindBuffer(GL_ARRAY_BUFFER, newMesh->buffersId[RMesh::normal]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * newMesh->buffersSize[RMesh::normal] * 3, newMesh->normals, GL_STATIC_DRAW);
	}

	if (newMesh->textureCoords != nullptr)
	{
		glGenBuffers(1, (GLuint*)&newMesh->buffersId[RMesh::texture]);
		glBindBuffer(GL_ARRAY_BUFFER, newMesh->buffersId[RMesh::texture]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * newMesh->buffersSize[RMesh::texture] * 2, newMesh->textureCoords, GL_STATIC_DRAW);
	}

}

void MRenderer3D::CreateChekerTexture()
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

	checkersId = Importer::TextureImp::CreateTexture(checkerImage, 64, 64, GL_RGBA);

}

void MRenderer3D::DrawBoundingBox(RMesh* mesh)
{
	float3 corners[8];
	mesh->aabb.GetCornerPoints(corners);
	glBegin(GL_LINES);
		//Between-planes right
	glVertex3fv((GLfloat*)&corners[1]);
	glVertex3fv((GLfloat*)&corners[5]);
	glVertex3fv((GLfloat*)&corners[7]);
	glVertex3fv((GLfloat*)&corners[3]);

	//Between-planes left
	glVertex3fv((GLfloat*)&corners[4]);
	glVertex3fv((GLfloat*)&corners[0]);
	glVertex3fv((GLfloat*)&corners[2]);
	glVertex3fv((GLfloat*)&corners[6]);

	//Far plane horizontal
	glVertex3fv((GLfloat*)&corners[5]);
	glVertex3fv((GLfloat*)&corners[4]);
	glVertex3fv((GLfloat*)&corners[6]);
	glVertex3fv((GLfloat*)&corners[7]);

	//Near plane horizontal
	glVertex3fv((GLfloat*)&corners[0]);
	glVertex3fv((GLfloat*)&corners[1]);
	glVertex3fv((GLfloat*)&corners[3]);
	glVertex3fv((GLfloat*)&corners[2]);

	//Near plane vertical
	glVertex3fv((GLfloat*)&corners[1]);
	glVertex3fv((GLfloat*)&corners[3]);
	glVertex3fv((GLfloat*)&corners[0]);
	glVertex3fv((GLfloat*)&corners[2]);

	//Far plane vertical
	glVertex3fv((GLfloat*)&corners[5]);
	glVertex3fv((GLfloat*)&corners[7]);
	glVertex3fv((GLfloat*)&corners[4]);
	glVertex3fv((GLfloat*)&corners[6]);

	glEnd();
}

void MRenderer3D::DrawScenePlane(int size)
{
	glLineWidth(1.0f);
	glBegin(GL_LINES);

	for (int i = -size; i <= size; i++)
	{
		glVertex3d(i, 0,-size);
		glVertex3d(i, 0, size);
		glVertex3d(size, 0, i);
		glVertex3d(-size, 0, i);
	}
	glEnd();
}

void MRenderer3D::SwitchCullFace()
{
	glIsEnabled(GL_CULL_FACE) == false? glEnable(GL_CULL_FACE): glDisable(GL_CULL_FACE);
}

void MRenderer3D::SwitchDepthTest()
{
	if (glDepthTest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void MRenderer3D::SwitchLighting()
{
	if (glLighting)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
}

void MRenderer3D::SwitchTexture2d()
{
	if (glTexture2d)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
}

void MRenderer3D::SwitchColorMaterial()
{
	if (glColorMaterial)
		glEnable(GL_COLOR_MATERIAL);
	else
		glDisable(GL_COLOR_MATERIAL);
}
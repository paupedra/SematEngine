#include <vector>
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "Resource.h"
#include "glmath.h"

#include "MWindow.h"
#include "MCamera3D.h"
#include "MScene.h"
#include "MEditor.h"
#include "MRenderer3D.h"

#include "IMesh.h"
#include "ITexture.h"

#include "CMaterial.h"
#include "CCamera.h"

#include "RMesh.h"
#include "RMaterial.h"
#include "RTexture.h"

#include "Dependecies/SDL/include/SDL.h"

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

#include "Dependecies/MathGeoLib/include/MathGeoLib.h"
//#include "Dependecies/mmgr/mmgr.h"

MRenderer3D::MRenderer3D(bool start_enabled) : Module(start_enabled), context()
{
	glCullFace = true;
	glColorMaterial = true;
	glLighting = true;
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
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
		lights[0].Active(true);

		//glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glAlphaFunc(GL_GREATER, 0.3);

		//glDepthFunc(GL_LESS); //hmmmm
		glEnable(GL_STENCIL_TEST); //Stencil things
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	CreateChekerTexture();
	Importer::TextureImp::InitDevil();


	return ret;
}

// PreUpdate: clear buffer
updateStatus MRenderer3D::PreUpdate(float dt)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);

	glLoadMatrixf(App->camera->currentCamera->GetViewMatrix());

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		
	// light 0 on cam pos
	lights[0].SetPos(App->camera->currentCamera->GetPos().x, App->camera->currentCamera->GetPos().y, App->camera->currentCamera->GetPos().z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
updateStatus MRenderer3D::PostUpdate(float dt)
{
	//ImGui::ShowDemoWindow();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf((GLfloat*)&App->camera->currentCamera->frustum.ProjectionMatrix().Transposed());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	
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

	if(App->camera->currentCamera != nullptr)
		App->camera->currentCamera->SetAspectRatio(((float)width / (float)height));

}

void MRenderer3D::DrawMesh(RMesh* mesh, float4x4 transform, CMaterial* material,bool drawVertexNormals, bool drawBoundingBox,GameObject* gameObject)
{
	glStencilMask(0x00);

	bool doStencil = false;
	if (gameObject == App->scene->selectedObject && gameObject != nullptr) //Stencil selection
	{
		doStencil = true;
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
	}

	if (!IsObjectInScreen(gameObject) && App->camera->currentCamera->cull)
		return;

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
			if (material->GetTexture() != nullptr)
			{
				if (material->GetTexture()->GetId() != 0)
					glBindTexture(GL_TEXTURE_2D, material->GetTexture()->GetId());
					//glBindTexture(GL_TEXTURE_2D, checkersId);
				else
				{
					Color color = material->GetMaterial()->GetColor();
					glColor4f(color.r, color.g, color.b, color.a);
				}
			}
			else
			{
				Color color = material->GetMaterial()->GetColor();
				glColor4f(color.r, color.g, color.b, color.a);
			}
		}
		else
		{
			glColor4f(1, 1, 1, 1);
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

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (drawVertexNormals)
		DrawVertexNormals(mesh,transform);

	if (drawBoundingBox || drawAllBoundingBoxes)
	{
		if(gameObject != nullptr)

		if (gameObject->HasComponentType(ComponentType::MESH))
		{
			vec* corners = new vec[8];
			glColor4f(0.5, 0, 0.5, 1);
			gameObject->OBB.GetCornerPoints(corners);
			DrawBox(corners);
			glColor4f(0, 0.5, 0, 1);
			gameObject->AABB.GetCornerPoints(corners);
			DrawBox(corners);
			RELEASE_ARRAY(corners);
		}
	}

	if (doStencil)
	{
		DrawStencilScaled(mesh,transform,material,drawVertexNormals,drawBoundingBox,gameObject);
	}

	glColor4f(1, 1, 1, 1);
}

void MRenderer3D::DrawVertexNormals(RMesh* mesh,float4x4 transform)
{
	glPushMatrix();
	glMultMatrixf((float*)&transform.Transposed());

	//Draw Normals
	glBegin(GL_LINES);

	uint loops = mesh->buffersSize[RMesh::vertex];
	for (uint i = 0; i < loops; i += 3)
	{
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glVertex3f(mesh->vertices[i], mesh->vertices[i + 1], mesh->vertices[i + 2]);
		glVertex3f(mesh->vertices[i] + mesh->normals[i], mesh->vertices[i + 1] + mesh->normals[i + 1], mesh->vertices[i + 2] + mesh->normals[i + 2]);
	}

	glEnd();
	glPopMatrix();
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

void MRenderer3D::DrawBox(float3* corners)
{
	glDisable(GL_LIGHTING);
	//glColor4f(255, 255, 0, 255);
	//mesh->aabb.GetCornerPoints(corners);

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
	glColor4f(1, 1, 1, 1);

	SwitchLighting();
}

void MRenderer3D::DrawLine(float3 a, float3 b)
{
	glDisable(GL_LIGHTING);
	glColor4f(0, 1, 0, 1);
	glLineWidth(2.0f);
	glBegin(GL_LINES);

	glVertex3d(a.x, a.y, a.z);
	glVertex3d(b.x, b.y, b.z);

	glEnd();
	glColor4f(1, 1, 1, 1);
	glLineWidth(1.0f);
	SwitchLighting();
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

bool MRenderer3D::IsObjectInScreen(GameObject* gameObject)
{
	float3 corners[8];
	gameObject->AABB.GetCornerPoints(corners); // get the corners of the box into the corners array

	for (int plane = 0; plane < 6; ++plane) {

		int iInCount = 8;
		int iPtIn = 1;

		for (int i = 0; i < 8; ++i) {
			// test this point against the planes
			if (App->camera->cullingCamera->planes[plane].IsOnPositiveSide(corners[i])) { //<-- “IsOnPositiveSide” from MathGeoLib
				iPtIn = 0;
				--iInCount;
			}
		}
		// were all the points outside of plane p?
		if(iInCount == 0)
			return(false);
	}
	// we must be partly in then otherwise
	return(true);
}

void MRenderer3D::DrawStencilScaled(RMesh* mesh, float4x4 transform, CMaterial* material, bool drawVertexNormals, bool drawBoundingBox, GameObject* gameObject)
{
	glDisable(GL_LIGHTING);
	glColor4f(0.5, 0.5, 1, 1);

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	//glDisable(GL_DEPTH_TEST);
	float scaleFactor = 1.03f;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//------------------------------------------ Draw object bigger
	glPushMatrix();
	float3 position, scale;
	Quat rotation;
	float4x4 scaledTransform = transform;

	scaledTransform.Decompose(position, rotation, scale);
	scale *= scaleFactor;
	scaledTransform = float4x4::FromTRS(position, rotation, scale);

	glMultMatrixf((float*)&scaledTransform.Transposed());

	glLineWidth(2);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//glBindBuffer(GL_ARRAY_BUFFER, mesh->buffersId[RMesh::texture]);
	//glTexCoordPointer(2, GL_FLOAT, 0, nullptr);

	//glBindBuffer(GL_ARRAY_BUFFER, mesh->buffersId[RMesh::normal]);
	//glNormalPointer(GL_FLOAT, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->buffersId[RMesh::vertex]);
	glVertexPointer(3, GL_FLOAT, 0, nullptr);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->buffersId[RMesh::index]);
	glDrawElements(GL_TRIANGLES, mesh->buffersSize[RMesh::index], GL_UNSIGNED_INT, nullptr);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	//------------------------------------------

	glEnable(GL_DEPTH_TEST);
	SwitchLighting();
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
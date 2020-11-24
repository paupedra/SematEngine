#ifndef __MODULERENDERER3D_H__
#define __MODULERENDERER3D_H__

#include "Module.h"
#include "glmath.h"
#include "Globals.h"
#include "Light.h"
#include "Dependecies/SDL/include/SDL.h"
#include <vector>
#include "Dependecies/MathGeoLib/src/MathGeoLib.h"

class RMesh;
class RMaterial;

#define MAX_LIGHTS 8

class MRenderer3D : public Module
{
public:

	MRenderer3D(bool start_enabled = true);
	~MRenderer3D();

	bool Init();
	update_status PreUpdate(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();

	void OnResize(int width, int height);
	void DrawMesh(RMesh* mesh, float4x4 transform, RMaterial* material, bool drawVertexNormals = false, bool drawBoundingBox = false);
	void DrawVertexNormals(RMesh* mesh, float4x4 transform);
	void GenerateBuffers(RMesh* newMesh);
	void CreateChekerTexture();
	void DrawBoundingBox(RMesh* mesh);
	void DrawScenePlane(int size);

	void SwitchCullFace(); //This is ugly sorry
	void SwitchDepthTest();
	void SwitchLighting();
	void SwitchTexture2d();
	void SwitchColorMaterial();

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 normalMatrix;
	mat4x4 projectionMatrix;

	uint checkersId;	//Default texture id

	bool wireframeMode;

	bool glCullFace;
	bool glLighting;
	bool glColorMaterial;
	bool glTexture2d;
	bool glDepthTest;
};
#endif // __MODULERENDERER3D_H__
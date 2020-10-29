#ifndef __MODULERENDERER3D_H__
#define __MODULERENDERER3D_H__

#include "Module.h"
#include "glmath.h"
#include "Globals.h"
#include "Light.h"
#include "Dependecies/SDL/include/SDL.h"
#include <vector>
#include "Dependecies/MathGeoLib/src/MathGeoLib.h"

struct Mesh;

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();

	void OnResize(int width, int height);
	
	void DrawMesh(Mesh* mesh, float4x4 transform);

	void GenerateBuffers(Mesh* newMesh);

	void FileDropCheck();

	void CreateChekerTexture();

	void SwitchCullFace();
	void SwitchDepthTest();
	void SwitchLighting();
	void SwitchTexture2d();
	void SwitchColorMaterial();

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ProjectionMatrix;

	//Checkerid test
	uint checkersId;
	uint houseId;

	bool wireframeMode;

	bool glCullFace;
	bool glLighting;
	bool glColorMaterial;
	bool glTexture2d;
	bool glDepthTest;
};
#endif // __MODULERENDERER3D_H__
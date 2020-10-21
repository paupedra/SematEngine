#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

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
	
	void DrawMesh(Mesh* mesh);

	void GenerateBuffers(Mesh* newMesh);

	void FileDropCheck();

	void DrawCube();

	void SwitchCullFace();

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ProjectionMatrix;

	uint currentId;

	std::vector<Mesh*> meshes;

	bool cullFace;
	
};
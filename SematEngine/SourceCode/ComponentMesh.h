#ifndef __COMPONENTMESH__
#define __COMPONENTMESH__

//#include "I_Mesh.h"

class Component;
struct Mesh;

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* owner);
	ComponentMesh(GameObject* owner, char* path, Mesh* mesh);
	~ComponentMesh();

	void Update() override;
	void DrawInspector() override;

	void DrawMesh();
	char* GetPath()const;
private:

	Mesh* mesh = nullptr;
	char* path = nullptr;
	bool drawVertexNormals = false;

};
#endif //__COMPONENTMESH__
#ifndef __COMPONENTMESH__
#define __COMPONENTMESH__

class Component;
struct Mesh;

class ComponentMesh : public Component
{
public:
	ComponentMesh();
	~ComponentMesh();

	void Update() override;

	void DrawMesh();

private:

	Mesh mesh;

};
#endif //__COMPONENTMESH__
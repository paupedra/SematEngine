#ifndef __COMPONENTMESH__
#define __COMPONENTMESH__

class Component;
struct Mesh;

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* owner);
	ComponentMesh(GameObject* owner, char* path, Mesh* mesh);
	~ComponentMesh();

	void Update() override;
	void CleanUp() override;
	void DrawInspector() override;

	static inline ComponentType GetType() { return ComponentType::MESH; };

	void DrawMesh();
	char* GetPath()const;
private:

	Mesh* mesh = nullptr;
	char* path = nullptr;
	bool drawVertexNormals = false;

};
#endif //__COMPONENTMESH__
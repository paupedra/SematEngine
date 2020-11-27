#ifndef __COMPONENTMESH__
#define __COMPONENTMESH__

class Component;
class RMesh;

class CMesh : public Component
{
public:
	CMesh(GameObject* owner);
	CMesh(GameObject* owner, char* path, RMesh* mesh);
	~CMesh();

	void Update() override;
	void CleanUp() override;
	void DrawInspector() override;
	void DrawMesh();

	static inline ComponentType GetType() { return ComponentType::MESH; };

	char* GetPath()const;
	RMesh* GetMesh()const;

private:

	RMesh* mesh = nullptr;
	char* path = nullptr;
	bool drawVertexNormals = false;

};
#endif //__COMPONENTMESH__
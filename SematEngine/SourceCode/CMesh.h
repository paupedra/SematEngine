#ifndef __COMPONENTMESH__
#define __COMPONENTMESH__

class Component;
class RMesh;
class JsonNode;

class CMesh : public Component
{
public:
	CMesh(GameObject* owner);
	CMesh(GameObject* owner, char* path, RMesh* mesh);
	~CMesh();

	void Update(float dt) override;
	void CleanUp() override;
	void OnPlay() override {};
	void OnStop() override {};

	void Serialize(JsonNode* node)override;
	void Load(JsonNode* node)override;

	void DrawMesh();

	void OnSave(JsonNode* node);

	void SetMesh(RMesh* _mesh);

	static inline ComponentType GetType() { return ComponentType::MESH; };

	char* GetPath()const;
	RMesh* GetMesh()const;

private:

	RMesh* mesh = nullptr;
	char* path = nullptr;

public:
	bool drawVertexNormals = false;
	bool drawAABB = false;

};
#endif //__COMPONENTMESH__

class Component;

class ComponentMesh : public Component
{
public:
	ComponentMesh();
	~ComponentMesh();

	void Update() override;

	void DrawMesh();

private:

};
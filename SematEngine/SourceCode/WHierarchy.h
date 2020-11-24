class Window;
class GameObject;

class WHierarchy : public Window
{
public:

	WHierarchy(bool _active);
	~WHierarchy();

	void Init() override;
	void Draw() override;
	void DrawTree(GameObject* object);

	void CleanUp() override;

private:
	GameObject* draggedObject = nullptr;
};
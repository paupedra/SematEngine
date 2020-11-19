class Window;
class GameObject;

class w_Hierarchy : public Window
{
public:

	w_Hierarchy(bool _active);
	~w_Hierarchy();

	void Init() override;
	void Draw() override;
	void DrawTree(GameObject* object);

	void CleanUp() override;

private:
	GameObject* draggedObject = nullptr;
};
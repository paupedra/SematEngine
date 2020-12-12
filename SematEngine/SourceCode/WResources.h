class Window;

class WResources : public Window
{
public:

	WResources(bool _active);
	~WResources();

	void Init() override;
	void Draw() override;

	void CleanUp() override;

private:

};
class Window;

class WInspector : public Window
{
public:

	WInspector(bool _active);
	~WInspector();

	void Init() override;
	void Draw() override;

	void CleanUp() override;

private:

};

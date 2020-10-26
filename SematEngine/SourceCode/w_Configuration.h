
class Window;


class w_Configuration : public Window
{
public:

	w_Configuration(bool _active);
	~w_Configuration();

	void Draw() override;

	void UpdateFPS(int _FPS);
	void UpdateMS(int _ms);

	void CleanUp() override;

private:

	float FPS[20];
	float MS[20];
	int fps;

	char* inputs[100];
};
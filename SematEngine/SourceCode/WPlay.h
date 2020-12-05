class Window;

class WPlay : public Window
{
public:

	WPlay(bool _active);
	~WPlay();

	void Init() override;
	void Draw() override;

	void CleanUp() override;

private:

};
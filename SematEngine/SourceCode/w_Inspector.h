class Window;

class w_Inspector : public Window
{
public:

	w_Inspector(bool _active);
	~w_Inspector();

	void Init() override;
	void Draw() override;

	void CleanUp() override;

private:

};

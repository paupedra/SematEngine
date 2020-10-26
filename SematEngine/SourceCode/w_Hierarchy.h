class Window;

class w_Hierarchy : public Window
{
public:

	w_Hierarchy(bool _active);
	~w_Hierarchy();

	void Draw() override;

	void CleanUp() override;

private:

};
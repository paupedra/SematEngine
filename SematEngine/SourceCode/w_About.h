#include <vector>
#include <string>

//#include "Window.h"
class Window;

class w_About : public Window
{
public:

	w_About(bool _active);
	~w_About();

	void Init() override;
	void Draw() override;

	void CleanUp() override;

private:

	 std::string licenseString;
};
#include <vector>
#include <string>

//#include "Window.h"
class Window;

class WAbout : public Window
{
public:

	WAbout(bool _active);
	~WAbout();

	void Init() override;
	void Draw() override;

	void CleanUp() override;

private:

	 std::string licenseString;
};
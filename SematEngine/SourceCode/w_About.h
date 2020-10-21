#include <vector>
#include <string>

#include "Window.h"

class w_About : public Window
{
public:

	w_About(bool _active);
	~w_About();

	void Draw() override;

private:

	const char* licenseString;
};
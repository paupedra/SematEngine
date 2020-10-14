#include <vector>
#include <string>

#include "Window.h"

class w_Console : public Window
{
public:

	w_Console(bool _active);
	~w_Console();

	void Draw() override;

	void AddLog(char* text);

private:

	std::vector<char*> logs;
	bool scrollToBottom;

};

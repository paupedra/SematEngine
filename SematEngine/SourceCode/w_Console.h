#ifndef __WINDOW_CONSOLE__
#define __WINDOW_CONSOLE__


class Window; 
//#include "Window.h" //How the fuck won't this work

class w_Console : public Window
{
public:

	w_Console(bool _active);
	~w_Console();

	void Draw() override;

	void AddLog(char* text);

	void CleanUp() override;

private:

	std::vector<char*> logs;
	bool scrollToBottom;

};
#endif //__WINDOW_CONSOLE__
#ifndef __WINDOW_CONSOLE__
#define __WINDOW_CONSOLE__


class Window; 
//#include "Window.h" //How the fuck won't this work

enum class LogType
{
	ERRORS,
	INIT,
	NONE
};

struct Logs
{
	const char* text;
	LogType type;
};

class WConsole : public Window
{
public:

	WConsole(bool _active);
	~WConsole();

	void Init() override;
	void Draw() override;

	void AddLog(char* text);

	void CleanUp() override;

private:

	std::vector<Logs> logs;
	bool scrollToBottom = false;

};
#endif //__WINDOW_CONSOLE__
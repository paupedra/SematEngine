#ifndef __WINDOW__
#define __WINDOW__

class Window
{
public:
	Window(bool _active);
	~Window();

	void SetActive();

	virtual void Draw() = 0;

	bool IsActive() const;

public:
	bool active;
};

#endif //__WINDOW__
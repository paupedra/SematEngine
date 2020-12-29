#ifndef __WINDOW_GAME__
#define __WINDOW_GAME__

class Window;
class GameObject;

class WGame : public Window
{
public:

	WGame(bool _active);
	~WGame();

	void Init() override;
	void Draw() override;
	void DrawTree(GameObject* object);

	void CleanUp() override;

private:
	

}; 
#endif //__WINDOW_GAME__
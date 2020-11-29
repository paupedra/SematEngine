#include <vector>
#include <string>

//#include "Window.h"
class Window;
class RTexture;

class WAssets : public Window
{
public:

	WAssets(bool _active);
	~WAssets();

	void Init() override;
	void Draw() override;

	void DrawFolders(std::string path);
	void DrawFolder(const char* name);

	void DrawFilesInFolder(const char* path);
	void DrawFile(const char* name);

	void CleanUp() override;

private:

	std::string selectedFolder = "Assets";

	RTexture* folder = nullptr;
};
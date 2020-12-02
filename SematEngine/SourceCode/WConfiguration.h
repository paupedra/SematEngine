
class Window;
#include <string>

struct HardwareInfo
{
	int cacheLineSize;
	int cpuCount;
	int ram;
	std::string cpuVendor;
	std::string renderer;
	std::string openGlVersion;
};

class WConfiguration : public Window
{
public:

	WConfiguration(bool _active);
	~WConfiguration();

	void Init() override;
	void Draw() override;

	void UpdateFPS(int _FPS);
	void UpdateMS(int _ms);
	void AddInput(const char* input);

	void CleanUp() override;

	HardwareInfo* GetHardwareInfo();

private:

	float FPS[20];
	float MS[20];
	int fps;

	HardwareInfo hardware;

	std::vector<const char*> inputs;
	int maxInputs = 20;
	bool scrollToBottomInputs = false;
};
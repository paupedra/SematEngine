
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

class w_Configuration : public Window
{
public:

	w_Configuration(bool _active);
	~w_Configuration();

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
	int maxInputs;
	bool scrollToBottomInputs = false;
};
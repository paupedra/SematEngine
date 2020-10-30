#ifndef __COMPONENTTEXTURE__
#define __COMPONENTTEXTURE__

class Component;
struct Texture;

class ComponentTexture : public Component
{
public:
	ComponentTexture(GameObject* owner);
	ComponentTexture(GameObject* owner, char* path, Texture* mesh);
	~ComponentTexture();

	void Update() override;
	void DrawInspector() override;

	char* GetPath()const;
	Texture* GetTexture() const;

	bool IsEnabled();
	void SwitchEnabledTexture();
private:

	Texture* texture = nullptr;
	char* path;

};
#endif //__COMPONENTTEXTURE__
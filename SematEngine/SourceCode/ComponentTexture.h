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
	void CleanUp() override;
	void DrawInspector() override;

	char* GetPath()const;
	Texture* GetTexture() const;

	bool IsEnabled()const ;
	void SwitchEnabledTexture();
private:

	Texture* texture = nullptr;
	char* path = "";
	bool drawTexture = true;

};
#endif //__COMPONENTTEXTURE__
#ifndef __COMPONENTTEXTURE__
#define __COMPONENTTEXTURE__

class Component;
class RMaterial;
class ConfigNode;

class CMaterial : public Component
{
public:
	CMaterial(GameObject* owner);
	CMaterial(GameObject* owner, const char* path, RMaterial* material);
	~CMaterial();

	void Update() override;
	void CleanUp() override;
	void DrawInspector() override;

	void OnSave(ConfigNode* node);

	static inline ComponentType GetType() { return ComponentType::MATERIAL; };

	const char* GetPath()const;
	RMaterial* GetTexture() const;

	bool IsEnabled()const ;
	void SwitchEnabledTexture();

private:

	RMaterial* material = nullptr;
	const char* path = "";
	bool drawTexture = true;

};
#endif //__COMPONENTTEXTURE__
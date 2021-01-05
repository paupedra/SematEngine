#ifndef __COMPONENTTEXTURE__
#define __COMPONENTTEXTURE__

class Component;
class RMaterial;
class JsonNode;
class RTexture;

class CMaterial : public Component
{
public:
	CMaterial(GameObject* owner);
	CMaterial(GameObject* owner, const char* path, RMaterial* material = nullptr,RTexture* texture = nullptr);
	~CMaterial();

	void Update(float dt) override;
	void CleanUp() override;
	void OnPlay() override {};
	void OnStop() override {};
	void Serialize(JsonNode* node)override;

	void OnSave(JsonNode* node);

	static inline ComponentType GetType() { return ComponentType::MATERIAL; };

	void SetTexture(RTexture* texture);
	void SetMaterial(RMaterial* material);

	const char* GetPath()const;
	RMaterial* GetMaterial() const;
	RTexture* GetTexture()const;

	bool IsEnabled()const ;
	void SwitchEnabledTexture();

private:

	RMaterial* material = nullptr;
	const char* path = "";
public:
	bool drawTexture = true;
};
#endif //__COMPONENTTEXTURE__
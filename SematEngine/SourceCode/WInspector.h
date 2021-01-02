class Window;
class Component;
class CTransform;
class CMesh;
class CMaterial;
class CCamera;
class CAnimator;

struct AnimationClip;

class WInspector : public Window
{
public:

	WInspector(bool _active);
	~WInspector();

	void Init() override;
	void Draw() override;

	void DrawComponent(Component* component);
	void DrawTransform(CTransform* component);
	void DrawMesh(CMesh* component);
	void DrawMaterial(CMaterial* component);
	void DrawCamera(CCamera* component);
	void DrawAnimator(CAnimator* animator);

	void CleanUp() override;

private:
	float startKey = 0;
	float endKey = 0;
	float speed = 0;

	float transitionDuration = 0;
	AnimationClip* transitionClip = nullptr;
};

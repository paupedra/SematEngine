#ifndef __COMPONENTANIMATOR__
#define __COMPONENTANIMATOR__

class GameObject;
class Component;
class RAnimation;

class CAnimator : public Component
{
public:

	CAnimator(GameObject* owner);
	~CAnimator();

	void Update()override;
	void CleanUp() override;

	inline std::vector<RAnimation*> GetAnimations() const { return animations; };

private:
	std::vector<RAnimation*> animations;
	//std::map<std::string,GameObject*> linkedBones; //These are the game objects linked to the animations (linke dby name)

};
#endif //__COMPONENTTEXTURE__
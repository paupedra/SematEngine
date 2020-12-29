#ifndef __COMPONENTANIMATOR__
#define __COMPONENTANIMATOR__

#include <map>

class GameObject;
class Component;
class RAnimation;

class Bone;

class CAnimator : public Component
{
public:

	CAnimator(GameObject* owner);
	~CAnimator();

	void Update(float dt)override;
	void CleanUp() override;

	void AddAnimation(RAnimation* newAnimation);

	void SetCurrentAnimation(RAnimation* animation); //sets new current animation and links its bones?
	void LinkBones(); //Fills linked bones list with the game objects corresponding to them

	void DrawBones();
	void UpdateBones(); //Updated linked Bones position

	void UpdateBonePosition(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it,float time);
	void UpdateBoneRotation(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it, float time);

	void Play();
	void Pause();
	void Stop(); //Resets and pauses animtion

	double TicksToTime(double ticks); //converts animation ticks to equivalent time
	double TimeToTicks(double time); //converts animation time to equivalent ticks

	inline void SwitchDrawBones() { drawBones = !drawBones; };

	inline bool GetDrawBones()const { return drawBones; };
	inline RAnimation* GetCurrentAnimation()const { return currentAnimation; };

	inline double GetAnimationTicks()const { return currentAnimationTicks; };
	inline double GetAnimationTime()const { return currentAnimationTime; };
	double GetAnimationSpeed()const ;
	double GetAnimationDuration()const;
	const char* GetAnimationName()const ;

	std::vector<RAnimation*> GetAnimations() const { return animations; };

	uint GetAnimationsSize() const { return animations.size(); };
	RAnimation* GetAnimation(uint index) const { return animations[index]; }

private:
	std::vector<RAnimation*> animations;
	std::map<std::string,GameObject*> linkedBones; //These are the game objects linked to the animations (linke dby name)

	GameObject* rootBone = nullptr;

	bool drawBones = false;		//Modified through Inspector

	RAnimation* currentAnimation = nullptr;

	double currentAnimationTime = 0;
	double currentAnimationTicks = 0;

	float durationInSeconds = 0;

	bool paused = true;
};
#endif //__COMPONENTTEXTURE__
#ifndef __COMPONENTANIMATOR__
#define __COMPONENTANIMATOR__

#include <map>
#include <queue>

class GameObject;
class Component;
class RAnimation;
struct AnimationChop;

struct AnimationTransition
{
	AnimationTransition(AnimationChop* _chop,float _duration)
	{
		chop = _chop;
		duration = _duration;
	};

	AnimationChop* chop = nullptr;
	float duration = 0;
};

class Bone;

class CAnimator : public Component
{
public:

	CAnimator(GameObject* owner);
	~CAnimator();

	void Update(float dt)override;
	void CleanUp() override;

	void AddAnimation(RAnimation* newAnimation);
	void AddChop(float startKey, float endKey, float speed);
	void AddTransition(AnimationChop* chop, float duration);	//Adds an AnimationTransition to the transitionqueue

	void LinkBones(); //Fills linked bones list with the game objects corresponding to them
	void DrawBones();
	void UpdateBones(); //Updated linked Bones position
	void UpdateBonePosition(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it);
	void UpdateBoneScale(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it);
	void UpdateBoneRotation(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it);

	void Play();
	void Pause();
	void Stop(); //Resets and pauses animtion

	double TicksToTime(double ticks); //converts animation ticks to equivalent time
	double TimeToTicks(double time); //converts animation time to equivalent ticks

	inline void SwitchDrawBones() { drawBones = !drawBones; };

	void SetCurrentAnimation(RAnimation* animation); //sets new current animation and links its bones?
	void SetPlaybackSpeed(float speed);

	inline bool GetDrawBones()const { return drawBones; };
	inline RAnimation* GetCurrentAnimation()const { return currentAnimation; };

	inline double GetAnimationTicks()const { return currentAnimationTicks; };
	inline double GetAnimationTime()const { return currentAnimationTime; };
	inline double GetPlaybackSpeed()const { return playbackSpeed; } //returns the multiplier at which animation is being played
	inline float GetDurationInSeconds()const { return durationInSeconds; }
	double GetAnimationSpeed()const;
	double GetAnimationDuration()const;
	const char* GetAnimationName()const ;

	inline std::vector<RAnimation*> GetAnimations() const { return animations; };
	inline std::vector<AnimationChop> GetChops()const { return chops; };

	uint GetAnimationsSize() const { return animations.size(); };
	RAnimation* GetAnimation(uint index) const { return animations[index]; }

private:
	std::vector<RAnimation*> animations;
	std::map<std::string,GameObject*> linkedBones;		//These are the game objects linked to the animations (linke dby name)

	GameObject* rootBone = nullptr;
	RAnimation* currentAnimation = nullptr;

	std::vector<AnimationChop> chops;					//The different parts the total animation is "chopped" in
	std::queue<AnimationTransition> transitionQueue;	//Transitions to a different chop

	double currentAnimationTime = 0;
	double currentAnimationTicks = 0;

	float durationInSeconds = 0;
	float playbackSpeed = 1;
	bool paused = true;
	bool drawBones = false;		//Modified through Inspector
};
#endif //__COMPONENTTEXTURE__
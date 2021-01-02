#ifndef __COMPONENTANIMATOR__
#define __COMPONENTANIMATOR__

#include <map>
#include <queue>

class GameObject;
class Component;
class RAnimation;
struct AnimationClip;

enum class AnimatorState
{
	PLAYING,
	PAUSED,
	TRANSITIONING,
	NONE
};

struct AnimationTransition
{
	AnimationTransition(AnimationClip* _clip,float _duration)
	{
		clip = _clip;
		duration = _duration;
	};

	AnimationClip* clip = nullptr;
	float duration = 0;				//In seconds
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
	void AddClip(float startKey, float endKey, float speed);
	void AddTransition(AnimationClip* clip, float duration);	//Adds an AnimationTransition to the transitionqueue

	void LinkBones(); //Fills linked bones list with the game objects corresponding to them
	void DrawBones();
	void UpdateBones(); //Updated linked Bones position
	void UpdateBonePosition(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it);
	void UpdateBoneScale(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it);
	void UpdateBoneRotation(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it);

	void UpdateTransitionTime(float dt);
	void UpdateTransitionBones();

	void Play();
	void Pause();
	void Stop(); //Resets and pauses animtion

	double TicksToTime(double ticks); //converts animation ticks to equivalent time
	double TimeToTicks(double time); //converts animation time to equivalent ticks

	inline void SwitchDrawBones() { drawBones = !drawBones; };

	void SetCurrentAnimation(RAnimation* animation); //sets new current animation and links its bones?
	void SetCurrentClip(AnimationClip* clip);
	void SetPlaybackSpeed(float speed);

	inline bool GetDrawBones()const { return drawBones; };
	inline RAnimation* GetCurrentAnimation()const { return currentAnimation; };
	inline AnimationClip* GetCurrentClip()const { return currentClip; };

	inline double GetAnimationTicks()const	  { return currentClipTicks; };
	inline double GetAnimationTime()const	  { return currentClipTime; };
	inline double GetPlaybackSpeed()const	  { return playbackSpeed; } //returns the multiplier at which animation is being played
	inline float  GetDurationInSeconds()const { return durationInSeconds; }

	double GetAnimationSpeed()const;
	double GetAnimationDuration()const;
	const char* GetAnimationName()const ;

	inline std::vector<RAnimation*> GetAnimations() const { return animations; };
	std::vector<AnimationClip>* GetClips();

	uint GetAnimationsSize() const { return animations.size(); };
	RAnimation* GetAnimation(uint index) const { return animations[index]; }

private:
	std::vector<RAnimation*> animations;
	std::map<std::string,GameObject*> linkedBones;		//These are the game objects linked to the animations (linke dby name)

	GameObject* rootBone = nullptr;
	RAnimation* currentAnimation = nullptr;
	AnimationClip* currentClip = nullptr;
	AnimationTransition* currentTransition = nullptr;

	std::queue<AnimationTransition> transitionQueue;	//Transitions to a different clip
	bool transitioning = false;
	float transitionTime = 0;							//Time the transition has been going on

	double transitionClipTicks = 0;						//Ticks the transition has been going on
	double transitionClipTime = 0;
	float transitionDurationInSeconds = 0;

	double currentClipTime = 0;
	double currentClipTicks = 0;

	float durationInSeconds = 0;
	float playbackSpeed = 1;
	bool paused = true;
	bool drawBones = false;		//Modified through Inspector

	AnimatorState state = AnimatorState::NONE;
};
#endif //__COMPONENTTEXTURE__
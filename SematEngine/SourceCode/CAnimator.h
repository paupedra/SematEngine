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
	void OnPlay() override;
	void OnStop() override;
	void Serialize(JsonNode* node)override;

	void AddAnimation(RAnimation* newAnimation);
	void AddClip(float startKey, float endKey, float speed);
	void AddTransition(AnimationClip* clip, float duration);	//Adds an AnimationTransition to the transitionqueue

	void LinkBones();											//Fills linked bones list with the game objects corresponding to them
	void DrawBones();											//Draws lines between bones

	// Bones Updates -------------------------------------------------------------------------------------------------------------
	void UpdateBones();											//Updated linked Bones position
	float3 UpdateBonePosition(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it);
	float3 UpdateBoneScale(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it);
	Quat UpdateBoneRotation(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it);

	void UpdateTransitionTime(float dt);
	void UpdateTransitionBones();
	void UpdateTransitionBonePosition(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it);
	void UpdateTransitionBoneScale(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it);
	void UpdateTransitionBoneRotation(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it);

	float3 ComputeBonePositionInterpolation(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it, double ticks, double time);
	float3 ComputeBoneScaleInterpolation(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it, double ticks, double time);
	Quat ComputeBoneRotationInterpolation(std::map<std::string, Bone>::const_iterator bone, std::map<std::string, GameObject*>::const_iterator it, double ticks, double time);

	void Play();
	void Pause();
	void Stop(); //Resets and pauses animtion

	double TicksToTime(double ticks); //converts animation ticks to equivalent time
	double TimeToTicks(double time); //converts animation time to equivalent ticks

	//Seters ---------------------------------------------------------------------------------------------------------------------
	inline void SwitchDrawBones() { drawBones = !drawBones; };

	inline void SetCollectionUID(UID _uid) { collectionUID = _uid; };
	void SetCurrentAnimation(RAnimation* animation); //sets new current animation and links its bones?
	void SetCurrentClip(AnimationClip* clip);
	void SetPlaybackSpeed(float speed);

	// Geters --------------------------------------------------------------------------------------------------------------------
	inline bool GetDrawBones()const { return drawBones; };
	inline RAnimation* GetCurrentAnimation()const { return currentAnimation; };
	inline AnimationClip* GetCurrentClip()const { return currentClip; };
	inline UID GetCollectionUID()const { return collectionUID; };
	inline double GetAnimationTicks()const	  { return currentClipTicks; };
	inline double GetAnimationTime()const	  { return currentClipTime; };
	inline double GetPlaybackSpeed()const	  { return playbackSpeed; } //returns the multiplier at which animation is being played
	inline float  GetDurationInSeconds()const { return durationInSeconds; }

	double GetAnimationSpeed()const;
	double GetAnimationDuration()const;
	const char* GetAnimationName()const ;

	std::vector<AnimationClip>* GetClips();

	inline std::vector<RAnimation*> GetAnimations() const { return animations; };
	uint GetAnimationsSize() const { return animations.size(); };
	RAnimation* GetAnimation(uint index) const { return animations[index]; }

private:
	std::vector<RAnimation*> animations;
	std::map<std::string,GameObject*> linkedBones;		//These are the game objects linked to the animations (linked by name)

	UID collectionUID = 0;
	GameObject* rootBone = nullptr;
	RAnimation* currentAnimation = nullptr;
	AnimationClip* currentClip = nullptr;
	AnimationTransition* currentTransition = nullptr;

	std::queue<AnimationTransition> transitionQueue;	//Transitions to a different clip
	
	float transitionTime = 0;							//Time the transition has been going on for

	double transitionClipTicks = 0;						//Ticks the transition has been going on
	double transitionClipTime = 0;
	float  transitionDurationInSeconds = 0;

	double currentClipTime = 0;
	double currentClipTicks = 0;
	float  durationInSeconds = 0;

	float playbackSpeed = 1;				// Factor the time is multiplied by
											
	bool transitioning = false;				// State booleans
	bool paused = true;
	bool drawBones = false;					//Modified through Inspector

	AnimatorState state = AnimatorState::NONE; //May be deleted
};
#endif //__COMPONENTTEXTURE__
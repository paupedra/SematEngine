#include "Module.h"
#include <vector>

class Primitive;
class GameObject;
class JsonNode;
class Component;

class CAnimator;

class MScene : public Module
{
public:
	MScene(bool start_enabled = true);
	~MScene();

	bool Start();
	updateStatus Update(float dt) override;
	updateStatus PostUpdate(float dt) override;
	bool CleanUp() override;
	bool Save(JsonNode* config) override;
	void OnPlay();
	void OnStop();

	uint SaveScene(bool isPlay); //Serialize and save scene, if isPlay saves it in playSavedScene
	uint SerializeScene(JsonNode* node);
	void LoadScene(uint sceneUid);
	void FindSavedScenes();

	GameObject* CreateGameObject(char* name, GameObject* parent= nullptr, bool isRoot = false);
	void SetSelectedObject(GameObject* gameObject);
	std::vector<GameObject*>::iterator FindGameObject(GameObject* gameObject);

	void PrepareToDestroyGameObject(GameObject* gameObject);
	void SetToDestroyGameObject(GameObject* gameObject);
	void EraseGameObject(std::vector<GameObject*>::iterator gameObject);

	void AddGameObject(GameObject* gameObject);

	uint GetNameRepeats(const char* name); //Return how many times this name is repeated on scene

	inline std::vector<UID> GetSavedScenes()const { return savedScenes; };

	//3rd Delivery temporary stuff
	void SetModifiedAnimation(CAnimator* animator);
	
public:
	std::vector<GameObject*> gameObjects;
	GameObject* rootObject = nullptr;
	GameObject* selectedObject = nullptr;

	UID playSavedScene = 0; //UID of the scene saved when hit play to be loaded on stop
	std::vector<UID> savedScenes;

	//3rd Delivery temporary stuff
	CAnimator* modifiedAnimation = nullptr;
};

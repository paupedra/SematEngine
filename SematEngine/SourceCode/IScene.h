#include <vector>
#include "Globals.h"

struct aiScene;
struct aiNode;
struct Texture;
class GameObject;
class ConfigNode;
class Component;

namespace Importer
{
	namespace SceneImporter
	{
		void Import(const char* file);

		void ProcessAiNode(const aiScene* scene,const aiNode* node,GameObject* parentObject, const char* file);

		const aiNode* ProcessTransform( const aiNode* node,GameObject* newGameObject);
		void ProcessMeshes(const aiScene* scene, const aiNode* node, GameObject* newGameObject);
		void ProcessMaterial(const aiScene* scene, const aiNode* node, GameObject* newGameObject, const char* file);

		//void Load(); //Load buffer from custom format file and store into mesh

	}
}
#include <vector>
#include "Globals.h"

struct aiScene;
struct aiNode;
struct Texture;
class GameObject;

namespace Importer
{
	namespace SceneImporter
	{
		void Import(const char* file);

		void ProcessAiNode(const aiScene* scene,const aiNode* node,GameObject* parentObject, const char* file);

		void LoadTransform( const aiNode* node,GameObject* newGameObject);
		void LoadMeshes(const aiScene* scene, const aiNode* node, GameObject* newGameObject);
		void LoadMaterial(const aiScene* scene, const aiNode* node, GameObject* newGameObject, const char* file);

		uint64 Save();	//Store mesh info into custom format file

		void Load(); //Load buffer from custom format file and store into mesh

	}
}
#include <vector>
#include "Globals.h"

struct aiNode;
class GameObject;

namespace Importer
{
	namespace SceneImporter
	{
		void Import(const char* file);

		void ProcessAiNode(const aiNode* node,GameObject* parentObject);

		void LoadTransform(const aiNode* node,GameObject* newGameObject);
		void LoadMeshes(const aiNode* node, GameObject* newGameObject);

		uint64 Save();	//Store mesh info into custom format file

		void Load(); //Load buffer from custom format file and store into mesh

	}
}
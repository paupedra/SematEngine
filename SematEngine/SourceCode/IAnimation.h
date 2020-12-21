#include "Globals.h"
#include <vector>

struct aiScene;
struct aiAnimation;
class RAnimation;

namespace Importer
{
	namespace AnimationImporter
	{
		uint Import(const aiAnimation* animation);

		void ImportAllAnimationsInScene(const aiScene* scene, std::vector<uint>& animations);

		uint64 Save(const RAnimation animation, const char* name);	//Store RAnimation into cff

		void Load(const char* fileBuffer, RAnimation* mesh); //Load buffer from custom format file and store into RAnimation
	}
}

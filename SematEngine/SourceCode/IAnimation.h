#include "Globals.h"
#include <vector>
#include <map>

struct aiScene;
struct aiAnimation;
class RAnimation;
struct Bone;

#include "Dependecies/MathGeoLib/include/Math/float3.h"
#include "Dependecies/MathGeoLib/include/Math/Quat.h"


namespace Importer
{
	namespace AnimationImporter
	{
		uint Import(const aiAnimation* animation);

		void ImportAllAnimationsInScene(const aiScene* scene, std::vector<uint>& animations);

		uint64 Save(RAnimation* animation, const char* name);	//Store RAnimation into cff

		void SaveBones(char** cursor, Bone bone);

		void Load(const char* fileBuffer, RAnimation* animation); //Load buffer from custom format file and store into RAnimation

		std::map<double, float3> LoadVector3Key(const char** cursor);

		std::map<double, Quat> LoadQuatKey(const char** cursor);
	}
}

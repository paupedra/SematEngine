#include "Globals.h"
#include <vector>

struct aiScene;
struct aiMaterial;

namespace Importer
{
	namespace MaterialImporter
	{
		uint Import(aiMaterial material);

		void ImportAllMaterialsInScene(const aiScene* scene, std::vector<uint>& material);

	}
}
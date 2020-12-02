#ifndef __MESHIMP__
#define __MESHIMG__

#include "Globals.h"
#include <vector>
#include "Dependecies/MathGeoLib/include/MathGeoLib.h"

struct aiScene;
struct aiNode;

namespace Importer
{
	namespace MeshImporter
	{
		std::vector<RMesh*> Import(const char* file);

		void LoadNodeMesh(const aiScene* scene, const aiNode* node, std::vector<RMesh*> &meshes);

		uint64 Save(const RMesh mesh,const char* name);	//Store mesh info into custom format file

		void Load(const char* fileBuffer, RMesh* mesh); //Load buffer from custom format file and store into mesh

	}
}

#endif //__MESHIMP__
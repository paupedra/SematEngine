#include <vector>
#include <map>
#include "Globals.h"

struct aiScene;
struct aiNode;
struct aiMaterial;
struct Texture;
class GameObject;
class JsonNode;
class Component;
class RScene;
struct RModel;
struct ModelNode;

namespace Importer
{
	namespace SceneImporter
	{
		void ImportSceneResource(const char* buffer, RScene* resource,uint size); //Imports .fbx file and store it in RScene
		void ProcessAiNodeModel(const aiScene* scene, const aiNode* node, RScene* _scene, UID parentUID);
		const aiNode* ProcessTransformModel(const aiNode* node, RModel* model);
		void ProcessMeshesModel(const aiScene* scene, const aiNode* node, RModel* model, RScene* _scene);
		void ProcessMaterialModel(const aiScene* scene, const aiNode* node, RModel* model);

		GameObject* LoadSceneResource(ModelNode node); //Loads scene into memory and hierarchy

		//void Load(); //Load buffer from custom format file and store into mesh
		uint ImportMaterial(aiMaterial* material, RMaterial* resource);
	}
}
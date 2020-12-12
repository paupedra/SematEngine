#include <vector>
#include "Globals.h"

struct aiScene;
struct aiNode;
struct Texture;
class GameObject;
class JsonNode;
class Component;
class RScene;
struct RModel;

namespace Importer
{
	namespace SceneImporter
	{
		void Import(const char* file);

		void ProcessAiNode(const aiScene* scene,const aiNode* node,GameObject* parentObject, const char* file);

		const aiNode* ProcessTransform( const aiNode* node,GameObject* newGameObject);
		void ProcessMeshes(const aiScene* scene, const aiNode* node, GameObject* newGameObject);
		void ProcessMaterial(const aiScene* scene, const aiNode* node, GameObject* newGameObject, const char* file);

		void ImportSceneResource(const char* buffer, RScene* resource,uint size); //Imports .fbx file and store it in RScene
		void ProcessAiNodeModel(const aiScene* scene, const aiNode* node, RScene* _scene, UID parentUID);
		const aiNode* ProcessTransformModel(const aiNode* node, RModel* model);
		void ProcessMeshesModel(const aiScene* scene, const aiNode* node, RModel* model, RScene* _scene);
		void ProcessMaterialModel(const aiScene* scene, const aiNode* node, RModel* model);

		//void Load(); //Load buffer from custom format file and store into mesh

	}
}
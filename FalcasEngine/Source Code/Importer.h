#pragma once
#include "Globals.h"
#include <string>
#include "External Libraries/MathGeoLib/include/Math/float4x4.h"
class ComponentMaterial;
class ResourceMaterial;
class ResourceModel;
class ComponentMesh;
class aiScene;
class aiNode;
class aiMesh;
class GameObject;

void DevilInit();

void DevilCleanUp();

void FreeImage(ResourceMaterial* res);

void ImportFBX(std::string file, uint ID);

void ImportGameObjectFromFBX(const aiScene* scene, aiNode* node, GameObject* parent, std::string file, uint ID, float4x4 transfor_heredated = float4x4::identity);

void ImportDefaultTexture(ComponentMaterial* mat);

ComponentMesh* ImportOnlyMesh(GameObject* game_object, std::string libraryPath, std::string assetPath, int meshNumber);

void GetAllMeshes(ResourceModel* mod, const aiScene* scene, aiNode* node, uint parent, std::string file);


namespace MeshImporter {
	void Import(const aiMesh* ai_material, char* name);
	uint Save(const ComponentMesh* mesh, char** filebuffer);
	void Load(const char* fileBuffer, ComponentMesh *mesh);
}

namespace MaterialImporter {
	void Import(std::string file, uint ID, ResourceMaterial* mat);
	uint Save(char** filebuffer);
	void Load(const char* fileBuffer, ResourceMaterial* res, uint size);
}

namespace ModelImporter {
	void Import(ResourceModel* mod, uint ID, std::string file);
	uint Save(ResourceModel* mod, char** buffer);
	void Load(const char* buffer, ResourceModel* mod);
}
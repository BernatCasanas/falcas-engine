#pragma once
#include "Globals.h"
#include <string>
#include "External Libraries/MathGeoLib/include/Math/float4x4.h"
class ComponentMaterial;
class ResourceMaterial;
class ResourceModel;
class ResourceMesh;
class ComponentMesh;
class aiScene;
class aiNode;
class aiMesh;
class GameObject;

void DevilInit();

void DevilCleanUp();

void FreeImage(ResourceMaterial* res);

void ImportDefaultTexture(ComponentMaterial* mat);

ComponentMesh* ImportOnlyMesh(GameObject* game_object, std::string libraryPath, std::string assetPath, int meshNumber);

void GetAllMeshes(ResourceModel* mod, const aiScene* scene, aiNode* node, uint parent, std::string file, float4x4 transform);


namespace MeshImporter {
	void Import(const aiMesh* ai_material, uint ID, char* name);
	uint Save(const ResourceMesh* mesh, char** filebuffer);
	void Load(const char* fileBuffer, ResourceMesh *mesh);
}

namespace MaterialImporter {
	void Import(std::string file, uint ID);
	uint Save(char** filebuffer);
	void Load(const char* fileBuffer, ResourceMaterial* res, uint size);
}

namespace ModelImporter {
	void Import(std::string file, uint ID);
	uint Save(ResourceModel* mod, char** buffer);
	void Load(const char* buffer, ResourceModel* mod);
}
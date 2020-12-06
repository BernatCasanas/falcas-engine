#pragma once
#include "Globals.h"
#include <string>
#include "External Libraries/MathGeoLib/include/Math/float4x4.h"
class ComponentMaterial;
class ResourceMaterial;
class ComponentMesh;
class aiScene;
class aiNode;
class aiMesh;
class GameObject;

void DevilInit();

void DevilCleanUp();

void ImportFBX(std::string file, uint ID);

void ImportGameObjectFromFBX(const aiScene* scene, aiNode* node, GameObject* parent, std::string file, uint ID, float4x4 transfor_heredated = float4x4::identity);

void ImportDefaultTexture(ComponentMaterial* mat);

ComponentMesh* ImportOnlyMesh(GameObject* game_object, std::string libraryPath, std::string assetPath, int meshNumber);

namespace MeshImporter {
	void Import(const aiMesh* ai_material, char* name);
	uint Save(const ComponentMesh* mesh, char** filebuffer);
	void Load(const char* fileBuffer, ComponentMesh *mesh);
}

namespace MaterialImporter {
	void Import(std::string file, uint ID);
	uint Save(char** filebuffer);
	void Load(const char* fileBuffer, ResourceMaterial* res, uint size);
}

namespace ModelImporter {

}
#pragma once
#include "Globals.h"
#include <string>
#include "External Libraries/MathGeoLib/include/Math/float4x4.h"
class ComponentMaterial;
class ComponentMesh;
class aiScene;
class aiNode;
class aiMesh;
class GameObject;

void DevilInit();

void DevilCleanUp();

void ImportFBX(std::string file, bool do_not_import_textures = false);

void ImportGameObjectFromFBX(const aiScene* scene, aiNode* node, GameObject* parent, std::string file, float4x4 transfor_heredated = float4x4::identity, bool do_not_import_textures = false);

void ImportDefaultTexture(ComponentMaterial* mat);

ComponentMesh* ImportOnlyMesh(GameObject* game_object, std::string libraryPath, std::string assetPath, int meshNumber);

namespace MeshImporter {
	int Import(const aiMesh* ai_material, ComponentMesh* mesh, char* name,  bool imported);
	uint Save(const ComponentMesh* mesh, char** filebuffer);
	void Load(const char* fileBuffer, ComponentMesh *mesh);
}

namespace TextureImporter {
	void Import(std::string file);
	uint Save(char** filebuffer);
	void Load(const char* fileBuffer, ComponentMaterial* mat, uint size);
}

namespace ModelImporter {

}
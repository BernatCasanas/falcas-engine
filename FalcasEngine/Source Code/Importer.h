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

void ImportFBX(std::string file);

void ImportGameObjectFromFBX(const aiScene* scene, aiNode* node, GameObject* parent, std::string file, float4x4 transfor_heredated=float4x4::identity);

void ImportDefaultTexture(ComponentMaterial* mat);

namespace MeshImporter {
	int Import(const aiMesh* ai_material, ComponentMesh* mesh, char* name,  bool imported);
	uint Save(const ComponentMesh* mesh, char** filebuffer);
	void Load(const char* fileBuffer, ComponentMesh *mesh);
}

namespace TextureImporter {
	void Import(ComponentMaterial* mat, std::string file, bool imported);
	uint Save(const ComponentMaterial* mat, char** filebuffer);
	void Load(const char* fileBuffer, ComponentMaterial* mat, uint size);
}


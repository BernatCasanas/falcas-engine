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

void ImportTexture(std::string file, ComponentMaterial* mat);

void ImportDefaultTexture(ComponentMaterial* mat);

int ImportMesh(aiMesh* ai_mesh, ComponentMesh* mesh);


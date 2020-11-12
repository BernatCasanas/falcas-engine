#pragma once
#include "Globals.h"
#include <string>
class ComponentMaterial;
class ComponentMesh;
class aiScene;
class aiNode;
class aiMesh;
class GameObject;

void ImportFBX(std::string file);

void ImportGameObjectFromFBX(const aiScene* scene, aiNode* node, GameObject* parent, std::string file);

void ImportTexture(std::string file, ComponentMaterial* mat);

void ImportDefaultTexture(ComponentMaterial* mat);

int ImportMesh(aiMesh* ai_mesh, ComponentMesh* mesh);


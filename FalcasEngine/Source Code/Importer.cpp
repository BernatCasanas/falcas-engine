#include "Importer.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include <stdio.h>
#include "FileSystem.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"
#include "ResourceMaterial.h"
#include "ResourceModel.h"
#include "External Libraries/Devil/Include/ilut.h"
#include "External Libraries/Assimp/Assimp/include/cimport.h"
#include "External Libraries/Assimp/Assimp/include/postprocess.h"
#include "External Libraries/Assimp/Assimp/include/scene.h"
#include "Resource.h"
#include "External Libraries/MathGeoLib/include/Math/Quat.h"
#include "ModuleResources.h"
#include "ResourceMesh.h"
#include <map>


#pragma comment (lib, "Source Code/External Libraries/Devil/lib/DevIL.lib")
#pragma comment (lib, "Source Code/External Libraries/Devil/lib/ILUT.lib")
#pragma comment (lib, "Source Code/External Libraries/Devil/lib/ILU.lib")


#pragma comment (lib, "Source Code/External Libraries/Assimp/Assimp/libx86/assimp.lib")


void DevilInit()
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	ilEnable(IL_ORIGIN_SET);
}

void DevilCleanUp()
{
	ilDisable(IL_ORIGIN_SET);
}

void FreeImage(ResourceMaterial* res)
{
	ilBindImage(0);
	ilDeleteImages(1, &res->image_name);
}

void ImportDefaultTexture(ComponentMaterial* mat) {
	GLubyte checker[64][64][4];

	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checker[i][j][0] = (GLubyte)c;
			checker[i][j][1] = (GLubyte)c;
			checker[i][j][2] = (GLubyte)c;
			checker[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &mat->defaultTex);
	glBindTexture(GL_TEXTURE_2D, mat->defaultTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, checker);
	glBindTexture(GL_TEXTURE_2D, 0);
}

ComponentMesh* ImportOnlyMesh(GameObject* game_object, std::string libraryPath, std::string assetPath, int meshNumber)
{
	char* buffer;
	const aiScene* scene = nullptr;
	ComponentMesh* mesh = (ComponentMesh*)game_object->CreateComponent(Component_Type::Mesh);
	aiMesh* ai_mesh = nullptr;
	bool imported = true;
	uint size = App->filesystem->LoadPath((char*)libraryPath.c_str(), &buffer);
	if (size == 0) {
		size = App->filesystem->Load(assetPath.c_str(), &buffer);
		scene = aiImportFileFromMemory(buffer, size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);
		aiNode* node = scene->mRootNode;
		ai_mesh = scene->mMeshes[meshNumber];
		imported = false;
		//mesh->file_name = libraryPath;
	}
	std::string s;
	libraryPath = "Library/Meshes/1860861522.falcasmesh";
	//MeshImporter::Load(App->filesystem->ReadPhysFile(libraryPath, size), mesh);

	aiReleaseImport(scene);
	return mesh;
}




void MeshImporter::Import(const aiMesh* ai_mesh, uint ID, char* name)
{
	ResourceMesh* mesh = new ResourceMesh(ID, Resource_Type::Mesh, name);
	mesh->materialIndex = -1;
	
	if (ai_mesh == nullptr)
	{
		LOG("Error loading FBX: %s", aiGetErrorString());
	}

	mesh->num_vertices = ai_mesh->mNumVertices * 3;
	mesh->vertices = new float[mesh->num_vertices];
	memcpy(mesh->vertices, ai_mesh->mVertices, sizeof(float) * mesh->num_vertices);
	LOG("Loading FBX correctly");
	LOG("New mesh with %d vertices", mesh->num_vertices / 3);

	if (ai_mesh->HasFaces())
	{
		mesh->num_indices = ai_mesh->mNumFaces * 3;
		mesh->indices = new uint[mesh->num_indices];
		for (uint j = 0; j < ai_mesh->mNumFaces; ++j)
		{
			if (ai_mesh->mFaces[j].mNumIndices != 3) {
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else {
				memcpy(&mesh->indices[j * 3], ai_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
			}
		}
		LOG("New mesh with %d index", mesh->num_indices);
	}
	mesh->num_normals = mesh->num_vertices;
	mesh->normals = new float[ai_mesh->mNumVertices * 3];
	for (int x = 0, y = 0; x < ai_mesh->mNumVertices; x++, y += 3) {
		if (ai_mesh->HasNormals())
		{
			mesh->normals[y] = ai_mesh->mNormals[x].x;
			mesh->normals[y + 1] = ai_mesh->mNormals[x].y;
			mesh->normals[y + 2] = ai_mesh->mNormals[x].z;
		}
	}

	if (ai_mesh->HasTextureCoords(0)) {
		mesh->num_textureCoords = ai_mesh->mNumVertices;
		mesh->texCoords = new float[mesh->num_textureCoords * 2];
		for (uint i = 0, j = 0; i < mesh->num_textureCoords; i++, j += 2) {
			mesh->texCoords[j] = ai_mesh->mTextureCoords[0][i].x;
			mesh->texCoords[j + 1] = ai_mesh->mTextureCoords[0][i].y;
		}
		mesh->materialIndex = ai_mesh->mMaterialIndex;
	}
	char* buffer;
	uint size = MeshImporter::Save(mesh, &buffer);
	App->filesystem->SaveInternal(name, buffer, size);
	delete mesh;
	delete[] buffer;

}

uint MeshImporter::Save(const ResourceMesh* mesh, char** filebuffer)
{
	uint ranges[4] = { mesh->num_indices, mesh->num_vertices, mesh->num_normals, mesh->num_textureCoords };

	uint size = sizeof(ranges) + sizeof(uint) * mesh->num_indices + sizeof(float) * mesh->num_vertices
		+ sizeof(float) * mesh->num_normals * 3 + sizeof(float) * mesh->num_textureCoords * 2;

	char* buffer = new char[size];
	char* cursor = buffer;

	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	//store indices
	bytes = sizeof(uint) * mesh->num_indices;
	memcpy(cursor, mesh->indices, bytes);
	cursor += bytes;

	//store vertices
	bytes = sizeof(float) * mesh->num_vertices;
	memcpy(cursor, mesh->vertices, bytes);
	cursor += bytes;

	//store normals
	bytes = sizeof(float) * mesh->num_normals;
	memcpy(cursor, mesh->normals, bytes);
	cursor += bytes;

	//store texcoords
	bytes = sizeof(float) * mesh->num_textureCoords * 2;
	memcpy(cursor, mesh->texCoords, bytes);

	*filebuffer = buffer;

	return size;
}

void MeshImporter::Load(const char* fileBuffer, ResourceMesh *mesh)
{

	char* cursor = (char*)fileBuffer;

	uint ranges[4];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	cursor += bytes;

	mesh->num_indices = ranges[0];
	mesh->num_vertices = ranges[1];
	mesh->num_normals = ranges[2];
	mesh->num_textureCoords = ranges[3];

	// Load indices
	bytes = sizeof(uint) * mesh->num_indices;
	mesh->indices = new uint[mesh->num_indices];
	memcpy(mesh->indices, cursor, bytes);
	cursor += bytes;

	//load vertices
	bytes = sizeof(float) * mesh->num_vertices * 3;
	mesh->vertices = new float[mesh->num_vertices * 3];
	memcpy(mesh->vertices, cursor, bytes);
	cursor += bytes;

	//load normals
	bytes = sizeof(float) * mesh->num_normals * 3;
	mesh->normals = new float[mesh->num_normals * 3];
	memcpy(mesh->normals, cursor, bytes);
	cursor += bytes;

	//load texcoords
	bytes = sizeof(float) * mesh->num_textureCoords * 2;
	mesh->texCoords = new float[mesh->num_textureCoords * 2];
	memcpy(mesh->texCoords, cursor, bytes);
	mesh->Initialize();

}

void MaterialImporter::Import(std::string file, uint ID)
{
	ResourceMaterial* res = new ResourceMaterial(ID,Resource_Type::Material,file);
	uint size;

	ilGenImages(1, &res->image_name);
	ilBindImage(res->image_name);

	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	
	char* buffer = nullptr;
	size = App->filesystem->Load(file.c_str(), &buffer);

	if (ilLoadL(IL_TYPE_UNKNOWN, buffer, size) == IL_FALSE) {
		ILenum error = ilGetError();
		LOG("Error loading Texture %s\n", iluErrorString(error));
	}

	delete[] buffer;

	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);


	res->height = ilGetInteger(IL_IMAGE_HEIGHT);
	res->width = ilGetInteger(IL_IMAGE_WIDTH);
	res->texture_id = ilutGLBindTexImage();

	char* texture_buffer = nullptr;
	res->size = MaterialImporter::Save(&texture_buffer);
	char name_buff[200];
	sprintf_s(name_buff, 200, "Library/Textures/%s.dds", std::to_string(ID).c_str());
	App->filesystem->SaveInternal(name_buff, texture_buffer, res->size);
	delete[] texture_buffer;
	
	ilBindImage(0);
	res->CleanUp();
	delete res;
}

uint MaterialImporter::Save(char** filebuffer)
{
	ILuint size;
	ILubyte* data;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
	size = ilSaveL(IL_DDS, nullptr, 0); // Get the size of the data buffer
	if (size > 0) {
		data = new ILubyte[size]; // allocate data buffer
		if (ilSaveL(IL_DDS, data, size) > 0) { // Save to buffer with the ilSaveIL function
			*filebuffer = (char*)data;
		}		
	}
	return size;
}

void MaterialImporter::Load(const char* fileBuffer, ResourceMaterial* res, uint size)
{
	char* buffer = (char*)fileBuffer;

	if (ilLoadL(IL_DDS, buffer, size) == IL_FALSE) {
		ILenum error = ilGetError();
		LOG("Error loading Texture %s\n", iluErrorString(error));
		ilBindImage(0);
		ilDeleteImages(1, &res->image_name);
	}
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	res->height = ilGetInteger(IL_IMAGE_HEIGHT);
	res->width = ilGetInteger(IL_IMAGE_WIDTH);
	res->texture_id = ilutGLBindTexImage();
	delete[] buffer;
}

void GetAllMeshes(ResourceModel* mod, const aiScene* scene, aiNode* node, uint parent, std::string file, float4x4 transform)
{
	if (node->mNumMeshes == 0 && node->mChildren == 0)
		return;

	aiVector3D position_ai, size_ai;
	aiQuaternion rotation_ai;
	node->mTransformation.Decompose(size_ai, rotation_ai, position_ai);
	float3 position = { position_ai.x,position_ai.y,position_ai.z };
	Quat rotation = { rotation_ai.x,rotation_ai.y,rotation_ai.z,rotation_ai.w };
	float3 _size = { size_ai.x,size_ai.y,size_ai.z };
	transform = transform * float4x4::FromTRS(position, rotation, _size);


	if (node->mNumMeshes > 0) {
		uint UUID = LCG().Int();
		aiMesh* ai_mesh = scene->mMeshes[node->mMeshes[0]];



		//Getting ID Texture
		aiString material_path;
		int num_material = ai_mesh->mMaterialIndex;
		scene->mMaterials[num_material]->GetTexture(aiTextureType_DIFFUSE, 0, &material_path);
		std::string path = App->filesystem->GetPathFile(file);
		path += material_path.C_Str();
		uint texUUID = 0;
		if (material_path.length > 0 && App->filesystem->FileExists(path)) {
			std::string metapath = path + ".meta";
			if (App->filesystem->FileExists(metapath)) {
				char* b;
				App->filesystem->LoadPath((char*)metapath.c_str(), &b);
				JsonObj obj(b);
				texUUID = obj.GetInt("ID");
				obj.CleanUp();
				delete[] b;
			}
			else {
				texUUID = LCG().Int();
				App->resources->CreateNewMetaFile(path, texUUID);
			}
		}
		//Insert Texture
		mod->textures[UUID] = texUUID;
		//Insert Transform
		mod->transform[UUID] = transform;
		//Insert Parent
		mod->meshes[UUID] = parent;

		std::string name_buff2 = std::to_string(UUID).c_str();
		char name_buff[200];
		sprintf_s(name_buff, 200, "Library/Meshes/%s.falcasmesh", name_buff2.c_str());

		MeshImporter::Import(ai_mesh, UUID, name_buff);
		App->resources->CreateNewMeshResource(UUID, file);
	}

	for (int i = 0; i < node->mNumChildren; ++i) {
		GetAllMeshes(mod, scene, node->mChildren[i], parent, file, transform);
	}
}

void ModelImporter::Import(std::string file, uint ID)
{
	ResourceModel* mod = new ResourceModel(ID, Resource_Type::Model, file);
	char* buffer;
	uint size = App->filesystem->LoadPath((char*)file.c_str(), &buffer);
	const aiScene* scene = nullptr;
	scene = aiImportFileFromMemory(buffer, size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);
	aiNode* node = scene->mRootNode;


	GetAllMeshes(mod, scene, node, ID, file, float4x4::identity);

	size = ModelImporter::Save(mod, &buffer);
	char name_buff[200];
	sprintf_s(name_buff, 200, "Library/Models/%s.falcasmodel", std::to_string(ID).c_str());
	App->filesystem->SaveInternal(name_buff, buffer, size);
	delete[] buffer;
	delete mod;
}

uint ModelImporter::Save(ResourceModel* mod, char** buffer)
{
	char* buf;

	JsonObj obj;
	JsonArray arr = obj.AddArray("items");

	for (std::map<uint, uint>::iterator it = mod->meshes.begin(); it != mod->meshes.end(); ++it) {
		JsonObj item;

		item.AddInt("ID", it._Ptr->_Myval.first);
		item.AddInt("TexID", mod->textures[it._Ptr->_Myval.first]);
		item.AddInt("ParentID", it._Ptr->_Myval.second);
		item.AddFloat4x4("Transform", mod->transform[it._Ptr->_Myval.first]);


		arr.AddObject(item);
	}
	uint size = obj.Save(&buf);
	*buffer = buf;
	obj.CleanUp();
	return size;
}

void ModelImporter::Load(const char* buffer, ResourceModel* mod)
{
	JsonObj obj(buffer);
	JsonArray arr = obj.GetArray("items");
	JsonObj item;

	for (int i = 0; i < arr.Size(); ++i) {
		item = arr.GetObjectAt(i);

		mod->meshes[item.GetInt("ID")] = item.GetInt("ParentID");
		mod->transform[item.GetInt("ID")] = item.GetFloat4x4("Transform");
		mod->textures[item.GetInt("ID")] = item.GetInt("TexID");

	}
	obj.CleanUp();
}

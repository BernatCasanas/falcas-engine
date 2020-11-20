#include "Importer.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include <stdio.h>
#include "FileSystem.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"
#include "External Libraries/Devil/Include/ilut.h"
#include "External Libraries/Assimp/Assimp/include/cimport.h"
#include "External Libraries/Assimp/Assimp/include/postprocess.h"
#include "External Libraries/Assimp/Assimp/include/scene.h"
#include "External Libraries/MathGeoLib/include/Math/Quat.h"

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

void ImportFBX(std::string file)
{
	const aiScene* scene = nullptr;
	char* buffer = nullptr;
	uint file_size = App->filesystem->Load(file.c_str(), &buffer);

	scene = aiImportFileFromMemory(buffer, file_size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);

	aiNode* nod = scene->mRootNode;

	ImportGameObjectFromFBX(scene, nod, App->scene_intro->root, file);

	App->filesystem->counterMesh = 0;

	aiReleaseImport(scene);
}

void ImportGameObjectFromFBX(const aiScene* scene, aiNode* node, GameObject* parent, std::string file, float4x4 transform_heredated)
{
	if (node->mNumMeshes == 0 && node->mNumChildren == 0)
		return;
	aiVector3D position_ai, size_ai;
	aiQuaternion rotation_ai;
	node->mTransformation.Decompose(size_ai, rotation_ai, position_ai);
	float3 position = { position_ai.x,position_ai.y,position_ai.z };
	Quat rotation = { rotation_ai.x,rotation_ai.y,rotation_ai.z,rotation_ai.w };
	float3 size = { size_ai.x,size_ai.y,size_ai.z };
	GameObject* game_object = parent;
	transform_heredated = transform_heredated * float4x4::FromTRS(position, rotation, size);
	transform_heredated.Decompose(position, rotation, size);
	if (node->mNumChildren > 1 || node->mNumMeshes != 0) {
		game_object = App->scene_intro->CreateGameObject(position, rotation, size, App->filesystem->GetFileName(file, true), parent);
		transform_heredated = float4x4::identity;
	}


	if (node->mNumMeshes > 0) {
		ComponentMesh* mesh = (ComponentMesh*)game_object->CreateComponent(Component_Type::Mesh);
		aiMesh* ai_mesh = scene->mMeshes[node->mMeshes[0]];
		bool mesh_imported = false;
		bool tex_imported = false;

		mesh->full_file_name = file;
		mesh->file_name = App->filesystem->GetFileName(file, true);

		char name_buff[200];
		if (App->filesystem->counterMesh != 0) {
			sprintf_s(name_buff, 200, "%s (%i)", mesh->file_name.c_str(), App->filesystem->counterMesh);
			mesh->file_name = name_buff;
		}
		App->filesystem->counterMesh++;

		int num_material = ai_mesh->mMaterialIndex;

		sprintf_s(name_buff, 200, "Library/Meshes/%s.falcasmesh", mesh->file_name.c_str());
		if (App->filesystem->FileExists(name_buff)) {
			mesh_imported = true;
		}
		MeshImporter::Import(ai_mesh, mesh, name_buff, mesh_imported);

		if (num_material != -1 && num_material < scene->mNumMaterials) {
			ComponentMaterial* mat = (ComponentMaterial*)game_object->CreateComponent(Component_Type::Material);
			aiString material_path;
			scene->mMaterials[num_material]->GetTexture(aiTextureType_DIFFUSE, 0, &material_path);
			if (material_path.length > 0) {
				std::string path = App->filesystem->GetPathFile(file);
				path += material_path.C_Str();
				sprintf_s(name_buff, 200, "Library/Textures/%s.falcastextures", mesh->file_name.c_str());
				if (App->filesystem->FileExists(name_buff)) {
					tex_imported = true;
				}
				TextureImporter::Import(mat, path, tex_imported);
			}
		}
	}
	for (int i = 0; i < node->mNumChildren; i++) {
		
			ImportGameObjectFromFBX(scene, node->mChildren[i], game_object, file,transform_heredated);
	}
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


int MeshImporter::Import(const aiMesh* ai_mesh, ComponentMesh* mesh, char* name, bool imported)
{
	int material_index = -1;
	if (!imported) {
		if (ai_mesh == nullptr)
		{
			const char* error = aiGetErrorString();
			LOG("Error loading FBX: %s", error);
			return material_index;
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
			material_index = ai_mesh->mMaterialIndex;
		}

		char* buffer;
		uint size = MeshImporter::Save(mesh, &buffer);
		char name_buff[200];
		sprintf_s(name_buff, 200, "Library/Meshes/%s.falcasmesh", mesh->file_name.c_str());
		App->filesystem->SaveInternal(name_buff, buffer, size);
	}

	else {
		char* buffer = App->filesystem->ReadPhysFile(name);
		MeshImporter::Load(buffer, mesh);
	}

	mesh->Initialization();


	return material_index;

}

uint MeshImporter::Save(const ComponentMesh* mesh, char** filebuffer)
{
	uint ranges[4] = { mesh->num_indices, mesh->num_vertices, mesh->num_normals, mesh->num_textureCoords };

	uint size = sizeof(ranges) + sizeof(uint) * mesh->num_indices + sizeof(float) * mesh->num_vertices * 3
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

void MeshImporter::Load(const char* fileBuffer, ComponentMesh *mesh)
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


}

void TextureImporter::Import(ComponentMaterial* mat, std::string file, bool imported)
{
	uint size;
	if (!imported) {
		mat->full_file_name = file;
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

		ilGenImages(1, &mat->image_name);
		ilBindImage(mat->image_name);

		char* buffer = nullptr;
		size = App->filesystem->Load(file.c_str(), &buffer);

		if (ilLoadL(IL_TYPE_UNKNOWN, buffer, size)) {
			ILenum error = ilGetError();
			LOG("Error loading Texture %s\n", iluErrorString(error));
		}
		mat->height = ilGetInteger(IL_IMAGE_HEIGHT);
		mat->width = ilGetInteger(IL_IMAGE_WIDTH);
		mat->show_default_tex = false;
		mat->texture_id = ilutGLBindTexImage();
		ilDeleteImages(1, &mat->image_name);

		mat->file_name = App->filesystem->GetFileName(mat->full_file_name);
		buffer = nullptr;
		size = TextureImporter::Save(mat, &buffer);
		char name_buff[200];
		sprintf_s(name_buff, 200, "Library/Textures/%s", mat->file_name.c_str());
		App->filesystem->SaveInternal(name_buff, buffer, size);
	}
	else {
		char* buffer = App->filesystem->ReadPhysFile(file);
		uint size = App->filesystem->GetSizePhysFile(file);
		TextureImporter::Load(buffer, mat, size);
	}
}

uint TextureImporter::Save(const ComponentMaterial* mat, char** filebuffer)
{
	ILuint size;
	ILubyte* data;
	ilBindImage(mat->texture_id);
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
	size = ilSaveL(IL_DDS, nullptr, 0); // Get the size of the data buffer
	if (size > 0) {
		data = new ILubyte[size]; // allocate data buffer
		if (ilSaveL(IL_DDS, data, size) > 0) { // Save to buffer with the ilSaveIL function
			*filebuffer = (char*)data;
			data = NULL;
			delete data;
		}
	}
	ilBindImage(0);
	return size;
}

void TextureImporter::Load(const char* fileBuffer, ComponentMaterial* mat, uint size)
{
	ILuint imageID = 0;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	char* buffer = (char*)fileBuffer;

	ilLoadL(IL_DDS, buffer, size);
	ilBindImage(0);
	ilDeleteImages(1, &imageID);

	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	mat->texture_id = (uint)(imageID);
	mat->width = ilGetInteger(IL_IMAGE_WIDTH);
	mat->height = ilGetInteger(IL_IMAGE_HEIGHT);

	ilBindImage(0);
}

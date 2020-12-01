#include "FileSystem.h"
#include <algorithm>
#include "Application.h"
#include "GameObject.h"
#include "ModuleSceneIntro.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "External Libraries/MathGeoLib/include/MathGeoLib.h"


#include "External Libraries/PhysFS/include/physfs.h"

#pragma comment(lib, "Source Code/External Libraries/PhysFS/libx86/physfs.lib")

FileSystem::FileSystem(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	PHYSFS_init(nullptr);
	PHYSFS_mount(".", nullptr, 1);
}

FileSystem::~FileSystem()
{
	PHYSFS_deinit();
}

bool FileSystem::Start()
{
	if (PHYSFS_isInit()) {
		LOG("Asset Manager is Loaded");
	}
	else {
		LOG("Failed to load Asset Manager");
	}



	PHYSFS_mount("Assets.zip", nullptr, 1);

	counterMesh = 0;

	return true;
}

std::string FileSystem::GetFileName(std::string file, bool has_filename_extension)
{
	if (has_filename_extension) {
		do {
			file.pop_back();
		} while (file.back() != '.');
		file.pop_back();
	}
	int pos = file.find_last_of('\\');
	if (pos == -1)
		pos = file.find_last_of('/');
	file = file.substr(pos + 1);
	return file;
}

std::string FileSystem::GetPathFile(std::string file)
{
	int pos = file.find_last_of('\\');
	if (pos == -1)
		pos = file.find_last_of('/');
	file = file.substr(0, pos+1);
	return file;
}

FILE_TYPE FileSystem::GetTypeFile(char* file)
{
	std::string name = file;


	uint size = name.find_last_of('.');
	name = name.substr(size + 1);


	if (name == "fbx" || name == "FBX") return FILE_TYPE::FBX;
	else if (name == "png" || name == "PNG") return FILE_TYPE::PNG;
	else if (name == "dds" || name == "DDS") return FILE_TYPE::DDS;
	else if (name == "scenefalcas" || name == "SCENE") return FILE_TYPE::SCENE;
	else return FILE_TYPE::UNKNOWN;
}

bool FileSystem::FileExists(std::string file)
{
	bool ret = false;
	if (PHYSFS_exists(file.c_str())) {
		ret = true;
	}
	return ret;
}

char* FileSystem::ReadPhysFile(std::string file)
{
	char* buffer = nullptr;
	PHYSFS_file* file_phys = PHYSFS_openRead(file.c_str());
	PHYSFS_sint32 size = (PHYSFS_sint32)PHYSFS_fileLength(file_phys);
	buffer = new char[size];
	PHYSFS_read(file_phys, buffer, 1, size);
	PHYSFS_close(file_phys);
	return buffer;
}

uint FileSystem::GetSizePhysFile(std::string file)
{
	PHYSFS_file* file_phys = PHYSFS_openRead(file.c_str());
	PHYSFS_sint32 size = (PHYSFS_sint32)PHYSFS_fileLength(file_phys);
	PHYSFS_close(file_phys);
	return size;
}

void FileSystem::DiscoverFiles(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list) const
{
	char** rc = PHYSFS_enumerateFiles(directory);
	char** i;

	std::string dir(directory);

	for (i = rc; *i != nullptr; i++)
	{
		if (PHYSFS_isDirectory((dir + *i).c_str()))
			dir_list.push_back(*i);
		else
			file_list.push_back(*i);
	}

	PHYSFS_freeList(rc);
}

void FileSystem::SaveInternal(const char* file, const void* buffer, uint size)
{
	PHYSFS_file* file_phys = PHYSFS_openWrite(file);
	PHYSFS_write(file_phys, buffer, 1, size);
	PHYSFS_close(file_phys);
}

void FileSystem::CreateFolders()
{
	CreateOneFolder("Library/Meshes/");
	CreateOneFolder("Library/Textures/");
	CreateOneFolder("Library/Config");
	CreateOneFolder("Library/Scenes");
}

void FileSystem::CreateOneFolder(char* name)
{
	if (PHYSFS_isDirectory(name) == false) {
		PHYSFS_mkdir(name);
	}
}

JsonObj FileSystem::GenerateConfigContent()
{
	JsonObj obj;
	obj.AddString("icon", "");
	JsonArray arr = obj.AddArray("configModules");
	JsonObj _obj;
	_obj.AddString("name", "moduleWindow");
	arr.AddObject(_obj);
	_obj.AddString("appName", "Falcas Engine");
	arr.AddObject(_obj);
	_obj.AddInt("height", 900);
	arr.AddObject(_obj);
	_obj.AddInt("width", 1600);
	arr.AddObject(_obj);
	_obj.AddBool("resizeable", true);
	arr.AddObject(_obj);
	_obj.AddBool("fullscreen", false);
	arr.AddObject(_obj);
	_obj.AddBool("border", true);
	arr.AddObject(_obj);

	return obj;
}

uint FileSystem::GenerateUUID()
{
	return LCG().Int();
}



uint FileSystem::Load(const char* path, char** buffer) const
{
	uint ret=0;
	std::string str = path;
	std::replace(str.begin(), str.end(), '\\', '/');
	int size = -1;
	size = str.find("Assets/");
	if(size>0)
		str=str.substr(size).c_str();
	const char* path_relative = str.c_str();
	PHYSFS_file* file = PHYSFS_openRead(path_relative);
	if (file!=nullptr&&!PHYSFS_eof(file))
	{
		uint lenght = PHYSFS_fileLength(file);
		*buffer = new char[lenght];
		uint bytes = PHYSFS_readBytes(file, *buffer, lenght);

		if (bytes != lenght)
		{
			LOG("%s", path, "ERROR: %s", PHYSFS_getLastError());
			if (buffer != NULL)  
			{               
				delete[] buffer;
				buffer = NULL;
			}
		}
		else
			ret = bytes;
	}
	else
		LOG("%s", path, "ERROR: %s", PHYSFS_getLastError());

	PHYSFS_close(file);

	return ret;
}

uint FileSystem::LoadPath(char* file, char** buffer) {
	PHYSFS_file* fs_file = PHYSFS_openRead(file);
	PHYSFS_sint32 size = (PHYSFS_sint32)PHYSFS_fileLength(fs_file);
	*buffer = new char[size + 1];
	PHYSFS_read(fs_file, *buffer, 1, size);
	(*buffer)[size] = '\0';
	PHYSFS_close(fs_file);
	return size;
}


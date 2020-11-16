#include "FileSystem.h"
#include <algorithm>
#include "Application.h"
#include "GameObject.h"
#include "ModuleSceneIntro.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"


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

	PHYSFS_setWriteDir(".");

	CreateFolders();

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
	else return FILE_TYPE::UNKNOWN;
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
}

void FileSystem::CreateOneFolder(char* name)
{
	if (PHYSFS_isDirectory(name) == false) {
		PHYSFS_mkdir(name);
	}
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


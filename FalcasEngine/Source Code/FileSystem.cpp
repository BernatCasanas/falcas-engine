#include "FileSystem.h"
#include <algorithm>

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

	return true;
}

uint FileSystem::Load(const char* path, char** buffer) const
{
	uint ret;
	std::string str = path;
	std::replace(str.begin(), str.end(), '\\', '/');
	int size = -1;
	size = str.find("Assets/");
	if(size>0)
		str=str.substr(size).c_str();
	const char* path_relative = str.c_str();
	PHYSFS_file* file = PHYSFS_openRead(path_relative);
	if (!PHYSFS_eof(file))
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


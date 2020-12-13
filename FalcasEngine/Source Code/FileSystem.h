#pragma once

#ifndef _FILE_SYSTEM_
#define _FILE_SYSTEM_


#include "Module.h"
#include "Globals.h"
#include <string>


enum class FILE_TYPE
{
	UNKNOWN = -1,
	FBX,
	PNG,
	DDS,
	TGA,
	SCENE,
	MESH,
	MODEL,
	META
};

class Application;
class FileSystem : public Module {
public:
	FileSystem(Application* app, bool start_enabled = true);
	~FileSystem();

	bool Start();
	update_status Update(float dt) { return UPDATE_CONTINUE; };
	bool CleanUp() { return true; };

	void SetWritenDir();

	std::string GetFileName(std::string file, bool has_filename_extension = false);
	std::string GetPathFile(std::string file);
	FILE_TYPE GetTypeFile(char* file);
	FILE_TYPE GetTypeFile(std::string file);


	bool FileExists(std::string file);
	void DeleteAFile(std::string file);
	char* ReadPhysFile(std::string file, uint& size);
	std::string CopyPhysFile(std::string file);

	uint GetSizePhysFile(std::string file);
	void DiscoverFiles(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list, std::string extension_to_ignore) const;
	void DiscoverFilesLibrary(const char* directory, std::vector<std::string>& file_list, std::vector<uint>& ids) const;

	std::vector<std::string> GetAllFiles(std::string file, std::vector<std::string> vector_file, std::string extension_file, bool ignore_extension, bool only_extension);

	void SaveInternal(const char* file, const void* buffer, uint size);

	uint GetLastModificationTime(std::string file);

	void CreateFolders();
	void CreateOneFolder(char* name);
	JsonObj GenerateConfigContent();

	uint GenerateUUID();

public:
	uint Load(const char* path, char** buffer) const;
	uint LoadPath(char* file, char** buffer);
	int counterMesh;
};


#endif // !_FILE_SYSTEM_

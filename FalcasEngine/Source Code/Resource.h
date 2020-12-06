#pragma once
#include "Globals.h"
#include <string>

enum class Resource_Type {
	None,
	Material,
	Model,
	Mesh
};

class Resource {
public:
	Resource(uint ID, Resource_Type type, std::string assets_file);
	virtual ~Resource();

	uint GetID() const;
	Resource_Type GetType() const;
	const char* GetAssetsFile() const;
	const char* GetLibraryFile() const;


public:
	uint referenceCount = 0;

protected:
	uint ID = 0;
	Resource_Type type;
	std::string assets_file;
	std::string library_file;
};
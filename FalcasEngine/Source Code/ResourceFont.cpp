#include "ResourceFont.h"


ResourceFont::ResourceFont(uint ID, Resource_Type type, std::string assets_file) : Resource(ID, type, assets_file)
{

}

bool ResourceFont::CleanUp()
{
	return true;
}

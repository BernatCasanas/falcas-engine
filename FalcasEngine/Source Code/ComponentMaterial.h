#ifndef _COMPONENT_MATERIAL_
#define _COMPONENT_MATERIAL_

#include "External Libraries/Devil/Include/ilut.h"
#include "Component.h"

#pragma comment (lib, "Source Code/External Libraries/Devil/lib/DevIL.lib")
#pragma comment (lib, "Source Code/External Libraries/Devil/lib/ILUT.lib")
#pragma comment (lib, "Source Code/External Libraries/Devil/lib/ILU.lib")

class GameObject;

class ComponentMaterial : public Component {
public:
	ComponentMaterial(GameObject* owner);
	~ComponentMaterial();
public:
	void LoadTexture(const char* file);
	void LoadDefault();


	void Inspector();

	
public:
	ILuint image_name;
	ILuint checkers;
	uint texture_id;
	uint defaultTex;
	std::string full_file_name;
	std::string file_name;

	bool show_default_tex = true;


};
#endif // !_COMPONENT_MATERIAL_
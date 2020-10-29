#include "Component.h"

#include "External Libraries/Devil/Include/ilut.h"

#pragma comment (lib, "DevIl/lib/DevIl.lib")
#pragma comment (lib, "DevIl/lib/ILUT.lib")
#pragma comment (lib, "DevIl/lib/ILU.lib")

class GameObject;

class ComponentMaterial : public Component {
public:
	ComponentMaterial(GameObject* parent);
	~ComponentMaterial();

public:
	void LoadTexture(const char* file);
	void LoadDefault();
	
public:
	ILuint image_name;
	ILuint checkers;
	uint texture_id;
	uint defaultTex;

	GameObject* parent;
};
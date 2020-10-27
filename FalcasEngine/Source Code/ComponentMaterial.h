#include "Component.h"


class GameObject;

class ComponentMaterial : public Component {
public:
	ComponentMaterial(GameObject* parent);
	~ComponentMaterial();

public:
	float width;
	float height;
	BYTE* pixmap;

};
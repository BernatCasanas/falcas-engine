#pragma once
#include "Component.h"

class ComponentImage : public Component {
public:
	ComponentImage(GameObject* owner);
	~ComponentImage();
	void Update();

	void Initialization();
	void Render();
	bool SaveComponent(JsonObj& obj);


	void Inspector();


};

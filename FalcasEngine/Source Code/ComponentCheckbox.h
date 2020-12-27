#pragma once
#include "Component.h"

class ComponentCheckbox : public Component {
public:
	ComponentCheckbox(GameObject* owner);
	~ComponentCheckbox();
	void Update();

	void Initialization();
	void Render();
	bool SaveComponent(JsonObj& obj);


	void Inspector();


};

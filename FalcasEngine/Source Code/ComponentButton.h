#pragma once
#include "Component.h"

class ComponentButton : public Component {
public:
	ComponentButton(GameObject* owner);
	~ComponentButton();
	void Update();

	void Initialization();
	void Render();
	bool SaveComponent(JsonObj& obj);


	void Inspector();


};

#pragma once
#include "Component.h"

class ComponentInputbox : public Component {
public:
	ComponentInputbox(GameObject* owner);
	~ComponentInputbox();
	void Update();

	void Initialization();
	void Render();
	bool SaveComponent(JsonObj& obj);

	
	void Inspector();


};
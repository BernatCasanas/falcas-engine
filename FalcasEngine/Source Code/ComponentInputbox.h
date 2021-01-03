#pragma once
#include "ComponentUI.h"
#include "Timer.h"

class ComponentTransform2D;

class ComponentInputbox : public ComponentUI {
public:
	ComponentInputbox(GameObject* owner, ComponentTransform2D* trans);
	~ComponentInputbox();
	void Update();

	void Initialization();
	void Render();
	bool SaveComponent(JsonObj& obj);

	void Backspace(int position);
	void Supr(int position);
	void CheckOutputText();

	void Inspector();


private:
	ComponentTransform2D* trans;
	std::string input_text = "";
	std::string output_text = "";
	int position = 0;
	bool text_input_activated = false;
	Timer output_timer;
};
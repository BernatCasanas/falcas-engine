#pragma once
#include "Globals.h"
#include <string>
#include "External Libraries/MathGeoLib/include/Math/float4.h"
class GameObject;

enum class Component_Type {
	Uknown=-1,
	Transform,
	Mesh,
	Material,
};

enum class Gui_Type {
	Unknwon=-1,
	Text,
	TextColored,
	CheckBox,
	SliderFloat,
	Tooltip,
	DragFloat,
	Separator,
	Column,
};

class Component {
public:
	Component(Component_Type type, GameObject* owner);
	virtual ~Component();
	virtual void Enable();
	virtual void Update();
	virtual void Disable();
	virtual bool Inspector(Gui_Type &type, int &index, std::string &info, bool* &checked, float* &number, bool &same_line, bool& separator_in_column, bool& next_column,
		int& num_columns, float& width, float4& color);

	Component_Type type;
	bool active;
	GameObject* owner;
	std::string name;

};
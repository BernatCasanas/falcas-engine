#include "Globals.h"
#include "External Libraries/MathGeoLib/include/Math/float3.h"
#include <vector>
enum class Component_Type;
class Component;
class GameObject {
public:
	GameObject(int id, std::string name, GameObject* parent, bool active = true);
	GameObject(int id);
	~GameObject();
	Component* CreateComponent(Component_Type type);
	std::string GetName() { return name; }
public:
	bool active;
	std::vector<Component*> components;
	GameObject* parent;
	std::vector<GameObject*> children;
	std::string name;
	int id;
private:
};

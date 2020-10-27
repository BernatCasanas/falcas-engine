#include "ComponentMaterial.h"
#include "Component.h"

ComponentMaterial::ComponentMaterial(GameObject* parent) : Component(Component_Type::Material, owner)
{

}

ComponentMaterial::~ComponentMaterial()
{
}

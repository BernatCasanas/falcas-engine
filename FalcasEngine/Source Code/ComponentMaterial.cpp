#include "ComponentMaterial.h"
#include "Component.h"
#include "External Libraries/Devil/Include/il.h"


ComponentMaterial::ComponentMaterial(GameObject* parent) : Component(Component_Type::Material, owner)
{
	name = "Material";
	//this->parent = parent;
}

ComponentMaterial::~ComponentMaterial()
{

}

void ComponentMaterial::LoadTexture(const char* file)
{
	ilGenImages(1, &image_name);
	ilBindImage(image_name);
	ilLoadImage(file);
	texture_id = ilutGLBindTexImage();
	ilDeleteImages(1, &image_name);
}

void ComponentMaterial::LoadDefault()
{
	GLubyte checker[64][64][4];

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &defaultTex);
	glBindTexture(GL_TEXTURE_2D, defaultTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, checker);
}

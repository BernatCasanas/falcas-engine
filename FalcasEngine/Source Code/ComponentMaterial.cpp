#include "ComponentMaterial.h"
#include "Component.h"


ComponentMaterial::ComponentMaterial(GameObject* parent) : Component(Component_Type::Material, owner)
{
	name = "Material";
	image_name = 0;
	checkers = 0;
	texture_id = 0;
	LoadDefault();

	this->parent = parent;
}

ComponentMaterial::~ComponentMaterial()
{

}

ComponentMaterial::ComponentMaterial() : Component(Component_Type::Material, owner)
{
	ILuint image_name = 0;
	ILuint checkers = 0;
	uint texture_id = 0;
	uint defaultTex = 0;
}

void ComponentMaterial::LoadTexture(const char* file)
{
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	ilGenImages(1, &image_name);
	ilBindImage(image_name);

	if (ilLoadImage(file)) {
		ILenum error = ilGetError();
		LOG("Error loading Texture %s\n", iluErrorString(error));
	}
	wantTex = true;
	texture_id = ilutGLBindTexImage();
	ilDeleteImages(1, &image_name);
}

void ComponentMaterial::LoadDefault()
{
	GLubyte checker[64][64][4];

	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checker[i][j][0] = (GLubyte)c;
			checker[i][j][1] = (GLubyte)c;
			checker[i][j][2] = (GLubyte)c;
			checker[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &defaultTex);
	glBindTexture(GL_TEXTURE_2D, defaultTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, checker);
}

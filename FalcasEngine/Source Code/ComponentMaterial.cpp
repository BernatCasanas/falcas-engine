#include "ComponentMaterial.h"
#include "Component.h"
#include "External Libraries/ImGui/imgui.h"

ComponentMaterial::ComponentMaterial(GameObject* owner) : Component(Component_Type::Material, owner)
{
	name = "Material";
	image_name = 0;
	checkers = 0;
	texture_id = 0;
	LoadDefault();

}

ComponentMaterial::~ComponentMaterial()
{

}



void ComponentMaterial::LoadTexture(const char* file)
{
	full_file_name = file;
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	ilGenImages(1, &image_name);
	ilBindImage(image_name);

	if (ilLoadImage(file)) {
		ILenum error = ilGetError();
		LOG("Error loading Texture %s\n", iluErrorString(error));
	}
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	width = ilGetInteger(IL_IMAGE_WIDTH);
	show_default_tex = false;
	texture_id = ilutGLBindTexImage();
	ilDeleteImages(1, &image_name);


	int pos = -1;
	pos = full_file_name.find_last_of('\\');
	if (pos == -1)
		pos = full_file_name.find_last_of('/');
	file_name = full_file_name.substr(pos + 1);
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

void ComponentMaterial::Inspector()
{
	ImGui::PushID(name.c_str());
	Component::Inspector();

	ImGui::Separator();

	ImGui::AlignTextToFramePadding();
	ImGui::Text("File: ");

	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), file_name.c_str());

	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip(full_file_name.c_str());
	}

	ImGui::Separator();


	ImGui::AlignTextToFramePadding();
	ImGui::Text("Size: ");

	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(std::to_string(width).c_str());

	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(" x ");

	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(std::to_string(height).c_str());


	ImGui::Image((void*)(intptr_t)texture_id, ImVec2((float)128, (float)128), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));


	ImGui::Separator();


	ImGui::Checkbox("Checkers Texture", &show_default_tex);

	ImGui::Separator();

	ImGui::PopID();
}

#include "ComponentFont.h"
#include "Application.h"
#include "ModuleResources.h"
#include "ResourceMaterial.h"
#include "ModuleUI.h"
#include "ModuleSceneIntro.h"
#include "ResourceMesh.h"
#include "External Libraries/ImGui/imgui.h"
#include "ComponentTransform2D.h"
#include "External Libraries/FreeType/include/freetype/freetype.h"
#include "ModuleCentralEditor.h"


ComponentFont::ComponentFont(GameObject* owner, ComponentTransform2D* trans) : ComponentUI(Component_Type::Font, owner, "Text"), trans(trans)
{
	Initialization();
}

ComponentFont::~ComponentFont()
{
	FT_Done_Face(face);
}

void ComponentFont::Update()
{
	if (is_clicked_first_frame) {
		OnTriggered(this);
	}
}

void ComponentFont::Initialization()
{
	FT_Error error;
	error = FT_New_Face(App->central_editor->ft, "Assets/Fonts/arial.ttf", 0, &face);
	if (error == FT_Err_Unknown_File_Format)
	{
		LOG("Font format not compatible");
	}
	else if (error)
	{
		LOG("The font doesnt exists");
	}

	FT_Set_Pixel_Sizes(face, 0, trans->GetSize().y);

	FT_Load_Char(face, 'X', FT_LOAD_RENDER);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

	for (unsigned char c = 0; c < 128; c++)
	{
		// load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			continue;
		}
		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// now store character for later use
		Character character = {
			texture,
			ImVec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			ImVec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		chars.insert(std::pair<char, Character>(c, character));
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void ComponentFont::Render()
{
	if (!active)
		return;
	
	

}

bool ComponentFont::SaveComponent(JsonObj& obj)
{
	return true;
}

void ComponentFont::Inspector()
{
	ImGui::PushID(name.c_str());
	Component::Inspector();
	ImGui::Separator();

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Text: ");
	ImGui::SameLine();
	ImGui::InputText("##text", &text, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll);

	ImGui::Separator();


		
	ImGui::PopID();
}

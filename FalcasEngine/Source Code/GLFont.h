#pragma once

#include "External Libraries/Glew/include/GL/glew.h"
#include "External Libraries/FreeType/include/ft2build.h"
#include "External Libraries/FreeType/include/freetype/freetype.h"

class GLFont {
public:
    GLFont(const char* fontFile);
    ~GLFont();

    void setFontFile(const char* fontFile);

    FT_Face getFaceHandle();

private:
    char* _fontFile;
    FT_Error _error;
    FT_Library _ft;
    FT_Face _face;

};


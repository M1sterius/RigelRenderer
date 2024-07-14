#include "FreeTypeUtility.hpp"
#include "vec2.hpp"
#include "glad.h"

#include <iostream>

#include "ft2build.h"
#include FT_FREETYPE_H

namespace rgr
{
	int GetFontGlyphsTextures(const std::string fontPath, unsigned int size, std::map<char, Character>* texturesMapOutput)
	{
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
		{
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
			return -1;
		}

		FT_Face face;
		if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
		{
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
			return -1;
		}

		FT_Set_Pixel_Sizes(face, 0, size);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

        for (unsigned char c = 0; c < 128; c++)
        {
            // load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            
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
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
            texturesMapOutput->insert(std::pair<char, Character>(c, character));
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
	}
}
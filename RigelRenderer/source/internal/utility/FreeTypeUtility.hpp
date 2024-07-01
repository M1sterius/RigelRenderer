#pragma once

#include "vec2.hpp"

#include <map>
#include <string>

namespace rgr
{
	struct Character
	{
		unsigned int textureID;
		glm::ivec2 size;
		glm::ivec2 bearing;
		unsigned int advance;
	};

	int GetFontGlyphsTextures(const std::string fontPath, unsigned int size, std::map<char, Character>* texturesMapOutput);
}

#include "Texture.hpp"

#include "stb_image.h"
#include "glew.h"
#include <iostream>
namespace rgr
{
	Texture::Texture(const std::string& path)
		: m_Handle(0), m_LocalBuffer(nullptr), m_Width(0), m_Height(0)
	{
		stbi_set_flip_vertically_on_load(1);
		m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, nullptr, 4);
		
		glGenTextures(1, &m_Handle);
		glBindTexture(GL_TEXTURE_2D, m_Handle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
		
		glBindTexture(GL_TEXTURE_2D, 0);

		if (m_LocalBuffer)
			stbi_image_free(m_LocalBuffer);
	}

    Texture::Texture(const size_t width, const size_t height, const Texture::TYPE type)
        : m_Handle(0), m_LocalBuffer(nullptr), m_Width(width), m_Height(height)
    {
        glGenTextures(1, &m_Handle);
        glBindTexture(GL_TEXTURE_2D, m_Handle);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        switch (type)
        {
            case TYPE::RGB:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width,
                             height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            case TYPE::RGBA:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width,
                             height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            case TYPE::RGBA32F:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width,
                             height, 0, GL_RGBA, GL_FLOAT, nullptr);
            case TYPE::DEPTH_COMPONENT:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width,
                             height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        }

        glBindTexture(GL_TEXTURE_2D, 0);
    }

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_Handle);
	}

	void Texture::Bind(unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_Handle);
	}

	void Texture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

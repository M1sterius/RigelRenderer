#include "Texture.hpp"

#include "stb_image.h"
#include "glad.h"

#include <iostream>

namespace rgr
{
	Texture::Texture(const std::string& path)
		: m_Handle(0), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_Path(path)
	{
        int width, height;
		stbi_set_flip_vertically_on_load(1);
		m_LocalBuffer = stbi_load(path.c_str(), &width, &height, nullptr, 4);

        m_Width = static_cast<size_t>(width);
        m_Height = static_cast<size_t>(height);

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
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, static_cast<int>(width),
                             static_cast<int>(height), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
                break;
            case TYPE::RGBA:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<int>(width),
                             static_cast<int>(height), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
                break;
            case TYPE::RGBA32F:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, static_cast<int>(width),
                             static_cast<int>(height), 0, GL_RGBA, GL_FLOAT, nullptr);
                break;
            case TYPE::DEPTH_COMPONENT:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, static_cast<int>(width),
                             static_cast<int>(height), 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
                break;
        }

        glBindTexture(GL_TEXTURE_2D, 0);
    }

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_Handle);
	}

	void Texture::BindToSlot(unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_Handle);
	}

    void Texture::Bind() const
    {
        glBindTexture(GL_TEXTURE_2D, m_Handle);
    }

	void Texture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

    void Texture::SetFilter(const Texture::FILTER min, const Texture::FILTER mag) const
    {
        this->Bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag));
        this->Unbind();
    }

    void Texture::SetWrap(const Texture::WRAP s, const Texture::WRAP t) const
    {
        this->Bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(s));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(t));
        this->Unbind();
    }

    void Texture::SetBorderColor(const float* color) const
    {
        this->Bind();
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
        this->Unbind();
    }
}

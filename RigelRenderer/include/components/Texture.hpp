#pragma once

#include <string>

namespace rgr
{
    
	class Texture
	{
	public:
        enum class TYPE
        {
            // Corresponds to GL_RGB format with GL_UNSIGNED_BYTE data type
            RGB,
            // Corresponds to GL_RGBA format with GL_UNSIGNED_BYTE data type
            RGBA,
            // Corresponds to GL_RGBA format with 32-bit GL_FLOAT data type
            RGBA32F,
            // Corresponds to GL_DEPTH_COMPONENT format with 32-bit GL_FLOAT data type
            DEPTH_COMPONENT
        };

        enum class FILTER
        {
            LINEAR = 0x2601,
            NEAREST = 0x2600
        };

        enum class WRAP
        {
            CLAMP_TO_BORDER = 0x812D,
            CLAMP_TO_EDGE = 0x812F,
            REPEAT = 0x2901
        };

		explicit Texture(const std::string& path);
        Texture(const size_t width, const size_t height, const TYPE type);
		~Texture();

		void BindToSlot(unsigned int slot = 0) const;
        void Bind() const;
		void Unbind() const;

		inline unsigned int GetHandle() const { return m_Handle; }
		inline size_t GetWidth() const { return m_Width; }
		inline size_t GetHeight() const { return m_Height; }
        inline std::string GetPath() const { return m_Path; }

        void SetFilter(const FILTER min, const FILTER mag) const;
        void SetWrap(const WRAP s, const WRAP t) const;
        void SetBorderColor(const float* color) const;
	private:
		unsigned int m_Handle;
		unsigned char* m_LocalBuffer;
		size_t m_Width, m_Height;
        std::string m_Path;
	};
}


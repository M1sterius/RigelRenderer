#pragma once

#include <string>

namespace rgr
{
    
	class Texture
	{
	public:
        enum class DATA_FORMAT
        {
            RGB,
            RGBA,
            DEPTH_COMPONENT
        };

        enum class DATA_TYPE
        {
            UNSIGNED_BYTE,
            BYTE,
            FLOAT,
        };

        enum class FILTER
        {
            LINEAR = 0x2601,
        };

        enum class WRAP
        {
            CLAMP_TO_EDGE,
            REPEAT
        };

		explicit Texture(const std::string& path);
		~Texture();

		void Bind(unsigned int slot = 0) const;
		void Unbind() const;

		inline unsigned int GetHandle() const { return m_Handle; }
		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }
		inline int GetBPP() const { return m_BPP; }
	private:
		unsigned int m_Handle;
		unsigned char* m_LocalBuffer;
		int m_Width, m_Height, m_BPP;
	};
}


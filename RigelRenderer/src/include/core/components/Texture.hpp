#pragma once

#include <string>

namespace rgr
{
	class Texture
	{
	public:
		Texture(const std::string& path);
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


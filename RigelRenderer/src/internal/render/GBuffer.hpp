#pragma once

#include <cstdio>

namespace rgr
{
	class GBuffer
	{
	private:
		unsigned int m_FBO;
		unsigned int m_PositionTexture;
		unsigned int m_NormalTexture;
		unsigned int m_ColorTexture;
		unsigned int m_DepthRBO;

		size_t m_ScreenWidth;
		size_t m_ScreenHeight;
	public:
		GBuffer(size_t screenWidth, size_t screenHeight);
		~GBuffer();

		void Bind() const;
		void Unbind() const;
		void Clear() const;

		void BindPositionTexture() const;
		void BindNormalTexture() const;
		void BindColorTexture() const;

		inline const unsigned int GetPositionTexture() const { return m_PositionTexture; }
		inline const unsigned int GetNormalTexture() const { return m_NormalTexture; }
		inline const unsigned int GetColorTexture() const { return m_ColorTexture; }
        inline const unsigned int GetFBOHandle() const {return m_FBO; }

        inline const size_t GetBufferWidth() const {return m_ScreenWidth; }
        inline const size_t GetBufferHeight() const {return m_ScreenHeight; }
	};
}

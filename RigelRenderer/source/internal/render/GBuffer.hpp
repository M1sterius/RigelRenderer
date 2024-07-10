#pragma once

#include <memory>

namespace rgr
{
    class Texture;

	class GBuffer
	{
	private:
		unsigned int m_FBO;
		unsigned int m_DepthRBO;

        std::unique_ptr<Texture> m_PositionTexture;
        std::unique_ptr<Texture> m_NormalTexture;
        std::unique_ptr<Texture> m_ColorTexture;

		size_t m_ScreenWidth;
		size_t m_ScreenHeight;
	public:
		GBuffer(size_t screenWidth, size_t screenHeight);
		~GBuffer();

		void Bind() const;
		void Unbind() const;
		void ClearColorDepthBufferBit() const;

		void BindPositionTexture() const;
		void BindNormalTexture() const;
		void BindColorTexture() const;

        inline const unsigned int GetFBOHandle() const {return m_FBO; }

        inline const size_t GetBufferWidth() const {return m_ScreenWidth; }
        inline const size_t GetBufferHeight() const {return m_ScreenHeight; }
	};
}

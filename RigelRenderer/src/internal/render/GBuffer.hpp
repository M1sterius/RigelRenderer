#pragma once

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
		GBuffer(const size_t screenWidth, const size_t screenHeight);
		~GBuffer();

		void Bind() const;
		void Unbind() const;

		void BindPositionTexture() const;
		void BindNormalTexture() const;
		void BindColorTexture() const;
	};
}
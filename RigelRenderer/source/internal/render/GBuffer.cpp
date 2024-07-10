#include "GBuffer.hpp"
#include "glew.h"
#include "Texture.hpp"


namespace rgr
{
	GBuffer::GBuffer(const size_t screenWidth, const size_t screenHeight)
		: m_ScreenWidth(screenWidth), m_ScreenHeight(screenHeight), m_FBO(0), m_DepthRBO(0)
	{
		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

        // Position buffer
        m_PositionTexture = std::make_unique<Texture>(m_ScreenWidth, m_ScreenHeight, rgr::Texture::TYPE::RGBA32F);
        m_PositionTexture->SetFilter(rgr::Texture::FILTER::NEAREST, rgr::Texture::FILTER::NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PositionTexture->GetHandle(), 0);

        // Normal buffer
        m_NormalTexture = std::make_unique<Texture>(m_ScreenWidth, m_ScreenHeight, rgr::Texture::TYPE::RGBA32F);
        m_NormalTexture->SetFilter(rgr::Texture::FILTER::NEAREST, rgr::Texture::FILTER::NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_NormalTexture->GetHandle(), 0);

        // Color buffer
        m_ColorTexture = std::make_unique<Texture>(m_ScreenWidth, m_ScreenHeight, rgr::Texture::TYPE::RGBA);
        m_ColorTexture->SetFilter(rgr::Texture::FILTER::NEAREST, rgr::Texture::FILTER::NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_ColorTexture->GetHandle(), 0);

		constexpr unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);

		// Depth buffer
		glGenRenderbuffers(1, &m_DepthRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_ScreenWidth, m_ScreenHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthRBO);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	GBuffer::~GBuffer()
	{
		glDeleteRenderbuffers(1, &m_DepthRBO);
		glDeleteFramebuffers(1, &m_FBO);
	}

	void GBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	}

	void GBuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GBuffer::ClearColorDepthBufferBit() const
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GBuffer::BindPositionTexture() const
	{
        m_PositionTexture->Bind(0);
	}

	void GBuffer::BindNormalTexture() const
	{
        m_NormalTexture->Bind(1);
	}

	void GBuffer::BindColorTexture() const
	{
        m_ColorTexture->Bind(2);
	}
}
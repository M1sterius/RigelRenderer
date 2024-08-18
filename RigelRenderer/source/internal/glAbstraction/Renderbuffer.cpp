#include "Renderbuffer.hpp"
#include "glad.h"

namespace rgr
{
    Renderbuffer::Renderbuffer(const size_t width, const size_t height, const Renderbuffer::Type type)
            : m_Handle(0), m_Width(width), m_Height(height), m_Type(type)
    {
        glGenRenderbuffers(1, &m_Handle);
        glBindRenderbuffer(GL_RENDERBUFFER, m_Handle);

        switch (m_Type)
        {
            case Type::DEPTH:
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
                                      static_cast<int>(m_Width), static_cast<int>(m_Height));
                break;
            case Type::DEPTH24_STENCIL8:
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                                      static_cast<int>(m_Width), static_cast<int>(m_Height));
                break;
        }

        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    Renderbuffer::~Renderbuffer()
    {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glDeleteRenderbuffers(1, &m_Handle);
    }
}

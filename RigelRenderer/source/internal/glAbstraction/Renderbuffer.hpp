#pragma once

namespace rgr
{
    class Renderbuffer
    {
    public:
        enum class Type
        {
            // Corresponds to GL_DEPTH_COMPONENT format with 32-bit GL_FLOAT data type
            DEPTH,
            // Corresponds to GL_DEPTH24_STENCIL8 format
            DEPTH24_STENCIL8
        };

        Renderbuffer(const size_t width, const size_t height, const Renderbuffer::Type type);
        ~Renderbuffer();

        inline unsigned int GetHandle() const { return m_Handle; }
        inline size_t GetWidth() const { return m_Width; }
        inline size_t GetHeight() const { return m_Height; }
        inline Renderbuffer::Type GetType() const { return m_Type; }
    private:
        unsigned int m_Handle;
        size_t m_Width, m_Height;
        Renderbuffer::Type m_Type;
    };
}

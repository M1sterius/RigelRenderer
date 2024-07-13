#include "IndexBuffer.hpp"
#include <glad/glad.h>

namespace rgr
{
	IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : m_Count(count)
	{
		glGenBuffers(1, &m_Handle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_Handle);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

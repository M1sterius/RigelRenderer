#include "VertexArray.hpp"

namespace rgr
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_Handle);
	}

	VertexArray::~VertexArray()
	{
		glBindVertexArray(0);
		for (size_t i = 0; i < m_vbHandles.size(); i++)
		{
			glDeleteBuffers(1, &m_vbHandles[i]);
		}
		glDeleteVertexArrays(1, &m_Handle);
	}

	void VertexArray::AddBuffer(const VertexBuffer& vb, const LayoutElement& le, const int bufferLayout)
	{
		glBindVertexArray(m_Handle);
		vb.Bind();

		m_vbHandles.push_back(vb.GetHandle());

		glEnableVertexAttribArray(bufferLayout);
		glVertexAttribPointer(bufferLayout, le.count, le.type, le.normalized, le.stride, (void*)le.offset);

		vb.Unbind();
		glBindVertexArray(0);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_Handle);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
}

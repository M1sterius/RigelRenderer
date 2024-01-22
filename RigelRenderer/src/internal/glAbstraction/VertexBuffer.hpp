#pragma once

namespace rgr
{
	class VertexBuffer
	{
	private:
		unsigned int m_Handle;
	public:
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

		inline unsigned int GetHandle() const { return m_Handle; }
	};
}


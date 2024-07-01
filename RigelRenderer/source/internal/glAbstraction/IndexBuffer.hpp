#pragma once

namespace rgr
{
	class IndexBuffer
	{
	private:
		unsigned int m_Handle;
		unsigned int m_Count;
	public:
		IndexBuffer(const unsigned int* data, unsigned int count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		inline unsigned int GetCount() const { return m_Count; }
		inline unsigned int GetHandle() const { return m_Handle; }
	};
}


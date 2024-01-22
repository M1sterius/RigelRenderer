#pragma once

#include "VertexBuffer.hpp"
#include "LayoutElement.hpp"

#include <vector>

namespace rgr
{
	class VertexArray
	{
	private:
		unsigned int m_Handle;
		std::vector<unsigned int> m_vbHandles;
	public:
		VertexArray();
		~VertexArray();

		void AddBuffer(const VertexBuffer& vb, const LayoutElement& le, const int bufferLayout);
		void Bind() const;
		void Unbind() const;

		inline unsigned int GetHandle() const { return m_Handle; }
	};
}


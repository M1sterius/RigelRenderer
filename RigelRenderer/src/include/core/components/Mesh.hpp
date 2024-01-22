#pragma once

#include <string>
#include <vector>

namespace rgr
{	
	class VertexArray;
	class IndexBuffer;

	class Mesh
	{
	public:
		Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const std::vector<float>& texCoords);
		Mesh(const std::string& objPath);
		~Mesh();

		inline rgr::VertexArray* GetVertexArray() const { return m_VertexArray; }
		inline rgr::IndexBuffer* GetIndexBuffer() const { return m_IndexBuffer; }
		inline int GetVertsCount() const { return m_VertsCount; }
		inline int GetTrisCount() const { return m_TrisCount; }
	private:
		rgr::VertexArray* m_VertexArray;
		rgr::IndexBuffer* m_IndexBuffer;

		int m_VertsCount = 0;
		int m_TrisCount = 0;
	};
}

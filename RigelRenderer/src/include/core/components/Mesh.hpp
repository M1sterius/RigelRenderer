#pragma once

#include <string>
#include <vector>

#define MESH_INDEXED 0
#define MESH_ARRAY 1

namespace rgr
{	
	class VertexArray;
	class IndexBuffer;

	/*
	Represents the geometry of an object
	*/
	class Mesh
	{
	private:
		rgr::VertexArray* m_VertexArray;
		rgr::IndexBuffer* m_IndexBuffer;

		int m_VertsCount = 0;
		int m_TrisCount = 0;
		int m_MeshType;
	public:
		/*
		Creates a mesh directly from vertex positions, indices and texture coordinates
		*/
		Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const std::vector<float>& texCoords);
		/*'
		Loads the mesh from an '.obj' file
		*/
		Mesh(const std::string& objPath);
		~Mesh();

		inline rgr::VertexArray* GetVertexArray() const { return m_VertexArray; }
		inline rgr::IndexBuffer* GetIndexBuffer() const { return m_IndexBuffer; }
		/*
		The number of vertices this mesh consists of
		*/
		inline int GetVertsCount() const { return m_VertsCount; }
		/*
		The number of triangles this mesh consists of
		*/
		inline int GetTrisCount() const { return m_TrisCount; }
		/*
		Returns the type of this mesh (either MESH_INDEXED or MESH_ARRAY)
		*/
		inline int GetMeshType() const { return m_MeshType; }
	};
}

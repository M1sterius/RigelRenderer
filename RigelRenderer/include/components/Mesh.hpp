#pragma once

#include "Internal.hpp"

#include <string>
#include <vector>
#include <memory>

namespace rgr
{
	class VertexArray;
	class IndexBuffer;

	/*
	Represents the geometry of an object
	*/
	class Mesh
	{
	public:
        enum class MeshType
        {
            INDEXED,
            ARRAY
        };

		/*
		Creates a mesh directly from vertex positions, indices and texture coordinates
		*/
		Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const std::vector<float>& texCoords);
		/*'
		Loads the mesh from an '.obj' file
		*/
		explicit Mesh(const std::string& objPath);
		~Mesh();
		
		/*
		The number of vertices this mesh consists of
		*/
		inline size_t GetVertsCount() const { return m_VertsCount; }
		/*
		The number of triangles this mesh consists of
		*/
		inline size_t GetTrisCount() const { return m_TrisCount; }
		/*
		Returns the type of this mesh (either MESH_INDEXED or MESH_ARRAY)
		*/
		inline MeshType GetMeshType() const { return m_MeshType; }

    private:
        std::unique_ptr<VertexArray> m_VertexArray;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;

        size_t m_VertsCount = 0;
        size_t m_TrisCount = 0;
        MeshType m_MeshType;
	INTERNAL:
		void Draw() const;

		static Mesh* Get2DQuadMesh();
	};
}

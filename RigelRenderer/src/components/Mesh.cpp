#include "glAbstraction/GlAbstraction.hpp"
#include "Mesh.hpp"
#include "obj_loader.h"
#include "Logger.hpp"

#include <vector>
#include <iostream>
#include <array>

namespace rgr
{
	Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const std::vector<float>& texCoords)
	{	
		m_MeshType = MeshType::INDEXED;

		m_VertexArray = new rgr::VertexArray();

		rgr::VertexBuffer verticesBuffer = rgr::VertexBuffer(vertices.data(), vertices.size() * sizeof(float));
		rgr::VertexBuffer texCoordsBuffer = rgr::VertexBuffer(texCoords.data(), texCoords.size() * sizeof(float));
		m_VertexArray->AddBuffer(verticesBuffer, Vec3Element, 0);
		m_VertexArray->AddBuffer(texCoordsBuffer, Vec2Element, 1);

		m_IndexBuffer = new rgr::IndexBuffer(indices.data(), indices.size());

		m_VertsCount = vertices.size() / 3;
		m_TrisCount = indices.size() / 3;
	}
	Mesh::Mesh(const std::string& objPath)
	{	
		m_MeshType = MeshType::INDEXED;

		objl::Loader loader = objl::Loader();
		
		if (!loader.LoadFile(objPath))
		{
			lgr::Error("Unable to find a '.obj' file: " + objPath);
		}

		std::vector<float> vertices;
		std::vector<float> texCoords;
		std::vector<float> normals;

		vertices.reserve(loader.LoadedVertices.size() * 3);
		texCoords.reserve(loader.LoadedVertices.size() * 2);
		normals.reserve(loader.LoadedVertices.size() * 3);

		for (const auto& vertex : loader.LoadedVertices)
		{
			vertices.push_back(vertex.Position.X);
			vertices.push_back(vertex.Position.Y);
			vertices.push_back(vertex.Position.Z);

			texCoords.push_back(vertex.TextureCoordinate.X);
			texCoords.push_back(vertex.TextureCoordinate.Y);

			normals.push_back(vertex.Normal.X);
			normals.push_back(vertex.Normal.Y);
			normals.push_back(vertex.Normal.Z);
		}

		m_VertexArray = new rgr::VertexArray();

		rgr::VertexBuffer verticesBuffer = rgr::VertexBuffer(vertices.data(), vertices.size() * sizeof(float));
		rgr::VertexBuffer texCoordsBuffer = rgr::VertexBuffer(texCoords.data(), texCoords.size() * sizeof(float));
		rgr::VertexBuffer normalsBuffer = rgr::VertexBuffer(normals.data(), normals.size() * sizeof(float));
	
		m_VertexArray->AddBuffer(verticesBuffer, Vec3Element, 0);
		m_VertexArray->AddBuffer(texCoordsBuffer, Vec2Element, 1);
		m_VertexArray->AddBuffer(normalsBuffer, Vec3Element, 2);

		m_IndexBuffer = new IndexBuffer(loader.LoadedIndices.data(), loader.LoadedIndices.size());
	
		m_VertsCount = loader.LoadedVertices.size() / 3;
		m_TrisCount = loader.LoadedIndices.size() / 3;
	}
	Mesh::~Mesh()
	{
		delete m_VertexArray;
		delete m_IndexBuffer;
	}

	Mesh* Mesh::Get2DQuadMesh()
	{
		static std::vector<float> quadVertices{
		-1.0f, 1.0f, 0.0f,
		 1.0f, 1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f
		};

		static std::vector<unsigned int> quadIndices{
		0, 1, 3,
		1, 2, 3
		};

		static std::vector<float> quadTexCoords{
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
		};

		static rgr::Mesh* quad = new rgr::Mesh(quadVertices, quadIndices, quadTexCoords);

		return quad;
	}
}

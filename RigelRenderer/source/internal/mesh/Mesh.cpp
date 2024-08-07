#include "Mesh.hpp"
#include "glAbstraction/GlAbstraction.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "glad.h"

#include <iostream>

namespace rgr
{
    Mesh::Mesh(const std::vector<float>& positions, const std::vector<float>& texCoords,
               const std::vector<float>& normals, const std::vector<uint32_t>& indices)
    {
        diffuseTexture = nullptr;
        specularTexture = nullptr;

        m_VertexArray = std::make_unique<rgr::VertexArray>();

        auto positionsBuffer = rgr::VertexBuffer(positions.data(), positions.size() * sizeof(float));
        auto texCoordsBuffer = rgr::VertexBuffer(texCoords.data(), texCoords.size() * sizeof(float));
        auto normalsBuffer = rgr::VertexBuffer(normals.data(), normals.size() * sizeof(float));

        m_VertexArray->AddBuffer(positionsBuffer, Vec3Element, 0);
        m_VertexArray->AddBuffer(texCoordsBuffer, Vec2Element, 1);
        m_VertexArray->AddBuffer(normalsBuffer, Vec3Element, 2);

        m_IndexBuffer = std::make_unique<rgr::IndexBuffer>(indices.data(), indices.size());

        m_VerticesCount = positions.size() / 3;
        m_TrisCount = indices.size() / 3;
    }

    Mesh::Mesh(const std::vector<float>& positions, const std::vector<float>& texCoords,
               const std::vector<uint32_t>& indices)
    {
        diffuseTexture = nullptr;
        specularTexture = nullptr;

        m_VertexArray = std::make_unique<rgr::VertexArray>();

        auto positionsBuffer = rgr::VertexBuffer(positions.data(), positions.size() * sizeof(float));
        auto texCoordsBuffer = rgr::VertexBuffer(texCoords.data(), texCoords.size() * sizeof(float));

        m_VertexArray->AddBuffer(positionsBuffer, Vec3Element, 0);
        m_VertexArray->AddBuffer(texCoordsBuffer, Vec2Element, 1);

        m_IndexBuffer = std::make_unique<rgr::IndexBuffer>(indices.data(), indices.size());

        m_VerticesCount = positions.size() / 3;
        m_TrisCount = indices.size() / 3;
    }

    Mesh::~Mesh() = default;

    void Mesh::Draw(const rgr::Shader& shader) const
    {
        if(diffuseTexture != nullptr)
            shader.BindTexture("u_DiffuseTexture", diffuseTexture, 0);
        if(specularTexture != nullptr)
            shader.BindTexture("u_SpecularTexture", diffuseTexture, 1);

        DrawElements();

        if (diffuseTexture != nullptr)
            diffuseTexture->Unbind();
        if (specularTexture != nullptr)
            specularTexture->Unbind();
    }

    void Mesh::DrawElements() const
    {
        m_VertexArray->Bind();
        m_IndexBuffer->Bind();

        glDrawElements(GL_TRIANGLES, static_cast<int>(m_IndexBuffer->GetCount()), GL_UNSIGNED_INT, nullptr);

        m_VertexArray->Unbind();
        m_IndexBuffer->Unbind();
    }

    rgr::Mesh& Mesh::GetBuiltInMesh(const Mesh::BUILT_IN_MESHES type)
    {
        static std::vector<float> quadPositions =
        {
            -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f
        };

        static std::vector<float> quadTexCoords =
        {
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f
        };

        static std::vector<unsigned int> quadIndices =
        {
            0, 1, 3,
            1, 2, 3
        };

        static auto quadMesh = rgr::Mesh(quadPositions, quadTexCoords, quadIndices);

        switch (type)
        {
            case BUILT_IN_MESHES::QUAD_NDC_FULLSCREEN:
                return quadMesh;
        }
    }
}

#pragma once

#include <vector>
#include <memory>

namespace rgr
{
    class Texture;
    class VertexArray;
    class VertexBuffer;
    class IndexBuffer;
    class Shader;

    class Mesh
    {
    public:
        Mesh(const std::vector<float>& positions, const std::vector<float>& texCoords, const std::vector<float>& normals,
             const std::vector<uint32_t>& indices);
        Mesh(const std::vector<float>& positions, const std::vector<float>& texCoords, const std::vector<uint32_t>& indices);
        ~Mesh();

        void DrawElements() const;
        void Draw(rgr::Shader& shader) const;

        inline size_t VerticesCount() const { return m_VerticesCount; }
        inline size_t TrisCount() const { return m_TrisCount; }

        rgr::Texture* diffuseTexture;
        rgr::Texture* specularTexture;

        enum class BUILT_IN_MESHES
        {
            QUAD,
        };

        static rgr::Mesh& GetBuiltInMesh(const BUILT_IN_MESHES type);
    private:
        std::unique_ptr<rgr::VertexArray> m_VertexArray;
        std::unique_ptr<rgr::IndexBuffer> m_IndexBuffer;

        size_t m_VerticesCount;
        size_t m_TrisCount;
    };
}

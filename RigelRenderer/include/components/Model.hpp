#pragma once

#include "Internal.hpp"

#include <string>
#include <vector>
#include <memory>

struct aiNode;
struct aiScene;
struct aiMesh;

namespace rgr
{
    class Mesh;
    class Shader;
    class Texture;

	class Model
	{
    public:
        explicit Model(const std::string& meshPath);
        ~Model();
    private:
        std::vector<rgr::Mesh*> m_Meshes;
        std::string m_Directory;

        void ProcessNode(aiNode* node, const aiScene* scene);
        rgr::Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
    INTERNAL:
        void DrawElements();
        void Draw(const Shader& shader);
	};
}

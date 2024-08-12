#include "Model.hpp"
#include "mesh/Mesh.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

namespace rgr
{
    Model::Model(const std::string& modelPath)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
            return;
        }
        m_Directory = modelPath.substr(0, modelPath.find_last_of('/'));

        ProcessNode(scene->mRootNode, scene);
    }

    Model::~Model() = default;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
    void Model::ProcessNode(aiNode* node, const aiScene* scene)
    {
        for (size_t i = 0; i < node->mNumMeshes; i++)
        {
            auto mesh = scene->mMeshes[node->mMeshes[i]];
            m_Meshes.push_back(ProcessMesh(mesh, scene));
        }
        for (size_t i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }
#pragma clang diagnostic pop

    rgr::Mesh* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<float> positions;
        std::vector<float> texCoords;
        std::vector<float> normals;
        std::vector<uint32_t> indices;

        positions.reserve(mesh->mNumVertices * 3);
        texCoords.reserve(mesh->mNumVertices * 2);
        normals.reserve(mesh->mNumVertices * 3);
        indices.reserve(mesh->mNumFaces);

        // Retrieve positions, texture coordinates and normals of each mesh vertex
        for (size_t i = 0; i < mesh->mNumVertices; i++)
        {
            positions.push_back(mesh->mVertices[i].x);
            positions.push_back(mesh->mVertices[i].y);
            positions.push_back(mesh->mVertices[i].z);

            if (mesh->mTextureCoords[0])
            {
                texCoords.push_back(mesh->mTextureCoords[0][i].x);
                texCoords.push_back(mesh->mTextureCoords[0][i].y);
            }
            else
            {
                texCoords.push_back(0.0f);
                texCoords.push_back(0.0f);
            }

            normals.push_back(mesh->mNormals[i].x);
            normals.push_back(mesh->mNormals[i].y);
            normals.push_back(mesh->mNormals[i].z);
        }

        // Retrieve indices of mesh triangles
        for(size_t i = 0; i < mesh->mNumFaces; i++)
        {
            auto face = mesh->mFaces[i];
            for(size_t j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        // Check if this mesh has a material
        if (mesh->mMaterialIndex >= 0)
        {
            auto resMesh = new rgr::Mesh(positions, texCoords, normals, indices);
            auto material = scene->mMaterials[mesh->mMaterialIndex];

            // Retrieve diffuse textures from mesh material
            if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
            {
                aiString diffuseTextureName;
                material->GetTexture(aiTextureType_DIFFUSE, 0, &diffuseTextureName);

                const std::string loadPath = m_Directory + '/' + diffuseTextureName.C_Str();

                // Check if the texture has already been loaded
                bool foundLoadedTexture = false;
                for (const auto loadedMesh : m_Meshes)
                {
                    if (loadedMesh->diffuseTexture->GetPath() == loadPath)
                    {
                        resMesh->diffuseTexture = loadedMesh->diffuseTexture;
                        foundLoadedTexture = true;
                        break;
                    }
                }
                if (!foundLoadedTexture)
                    resMesh->diffuseTexture = new rgr::Texture(loadPath);
            }

            // Retrieve specular textures from mesh material
            if (material->GetTextureCount(aiTextureType_SPECULAR) > 0)
            {
                aiString specularTextureName;
                material->GetTexture(aiTextureType_SPECULAR, 0, &specularTextureName);

                const std::string loadPath = m_Directory + '/' + specularTextureName.C_Str();

                // Check if the texture has already been loaded
                bool foundLoadedTexture = false;
                for (const auto loadedMesh : m_Meshes)
                {
                    if (loadedMesh->specularTexture->GetPath() == loadPath)
                    {
                        resMesh->specularTexture = loadedMesh->specularTexture;
                        foundLoadedTexture = true;
                        break;
                    }
                }
                if (!foundLoadedTexture)
                    resMesh->specularTexture = new rgr::Texture(loadPath);
            }

            return resMesh;
        }
        else
        {
            auto resMesh = new rgr::Mesh(positions, texCoords, normals, indices);
            resMesh->diffuseTexture = nullptr;
            resMesh->specularTexture = nullptr;
            return resMesh;
        }
    }

    void Model::DrawElements()
    {
        for (const auto mesh : m_Meshes)
            mesh->DrawElements();
    }

    void Model::Draw(const Shader& shader)
    {
        for (const auto mesh : m_Meshes)
            mesh->Draw(shader);
    }
}

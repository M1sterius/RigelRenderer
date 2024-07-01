#include "renderable/RenderableMesh.hpp"
#include "Logger.hpp"
#include "glAbstraction/GlAbstraction.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "glm.hpp"

#include <iostream>

namespace rgr
{
	RenderableMesh::RenderableMesh(rgr::Mesh* mesh)
		: m_Mesh(mesh)
	{
		diffuseTexture = nullptr;
		specularTexture = nullptr;
	}

	RenderableMesh::~RenderableMesh()
	{

	}

	void RenderableMesh::RenderDepth(const glm::mat4& lightSpaceMatrix)
	{
		rgr::Shader* depthMapShader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::DEPTH_MAP);

		depthMapShader->Bind();
		depthMapShader->SetUniformMat4("u_LightSpaceMatrix", false, lightSpaceMatrix);
		depthMapShader->SetUniformMat4("u_Model", false, GetTransform().GetModelMatrix());

		m_Mesh->Draw();
	}

	void RenderableMesh::RenderGeometry(rgr::Shader* shader, const glm::mat4& viewProj)
	{	
		const glm::mat4 model = GetTransform().GetModelMatrix();
		const glm::mat4 mvp = viewProj * model;

		shader->SetUniformMat4("u_MVP", true, mvp); // transpose
		shader->SetUniformMat4("u_Model", false, model); // no transpose
		shader->SetUniformMat3("u_NormalMatrix", false, GetTransform().GetNormalMatrix()); // no transpose

		if (diffuseTexture != nullptr)
			shader->BindTexture("u_DiffuseTexture", diffuseTexture, 0);
		if (specularTexture != nullptr)
			shader->BindTexture("u_SpecularTexture", specularTexture, 1);

		m_Mesh->Draw();
	}
}

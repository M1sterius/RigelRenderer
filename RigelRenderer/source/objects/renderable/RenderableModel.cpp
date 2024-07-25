#include "renderable/RenderableModel.hpp"
#include "Logger.hpp"
#include "glAbstraction/GlAbstraction.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "Model.hpp"
#include "Texture.hpp"
#include "glm.hpp"

#include <iostream>
#include <utility>

namespace rgr
{
	RenderableModel::RenderableModel(std::shared_ptr<rgr::Model> mesh)
		: m_Model(std::move(mesh))
	{

	}

	RenderableModel::~RenderableModel() = default;

	void RenderableModel::RenderDepth(const glm::mat4& lightSpaceMatrix)
	{
		auto& depthMapShader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::DEPTH_MAP);

		depthMapShader.Bind();
		depthMapShader.SetUniformMat4("u_LightSpaceMatrix", false, lightSpaceMatrix);
		depthMapShader.SetUniformMat4("u_Model", false, GetTransform().GetModelMatrix());

		m_Model->DrawElements();
	}

	void RenderableModel::RenderGeometry(const rgr::Shader& shader, const glm::mat4& viewProj)
	{	
		const glm::mat4 model = GetTransform().GetModelMatrix();
		const glm::mat4 mvp = viewProj * model;

		shader.SetUniformMat4("u_MVP", true, mvp); // transpose
		shader.SetUniformMat4("u_Model", false, model); // no transpose
		shader.SetUniformMat3("u_NormalMatrix", false, GetTransform().GetNormalMatrix()); // no transpose

		m_Model->Draw(shader);
	}
}

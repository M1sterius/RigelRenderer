#include "Renderable/RenderableMesh.hpp"
#include "Logger.hpp"
#include "glAbstraction/GlAbstraction.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "MaterialLit.hpp"
#include "Mesh.hpp"

#include "RenderUtility.hpp"
#include "internal.hpp"
#include "glm.hpp"

#include <iostream>
#include <vector>

namespace rgr
{
	RenderableMesh::RenderableMesh(rgr::Mesh* mesh, rgr::MaterialLit* material)
		: m_Mesh(mesh), m_Material(material)
	{
		
	}

	RenderableMesh::~RenderableMesh()
	{

	}

	void RenderableMesh::RenderDepth(const glm::mat4& lightSpaceMatrix)
	{
		rgr::Shader* depthMapShader = rgr::Shader::GetDepthMapShader();

		m_Mesh->Bind();

		depthMapShader->Bind();
		depthMapShader->SetUniformMat4("u_LightSpaceMatrix", false, lightSpaceMatrix);
		depthMapShader->SetUniformMat4("u_Model", false, GetTransform().GetModelMatrix());

		m_Mesh->Draw();
	}

	void RenderableMesh::RenderGeometry(const rgr::Shader* shader, const glm::mat4& viewProj)
	{

	}

}

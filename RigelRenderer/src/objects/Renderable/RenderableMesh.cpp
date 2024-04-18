#include "Renderable/RenderableMesh.hpp"
#include "Logger.hpp"
#include "glAbstraction/GlAbstraction.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "Mesh.hpp"

#include "RenderUtility.hpp"
#include "internal.hpp"
#include "glm.hpp"

#include <iostream>
#include <vector>

namespace rgr
{
	RenderableMesh::RenderableMesh(rgr::Mesh* mesh, rgr::Material* material)
		: m_Mesh(mesh), m_Material(material)
	{
		
	}

	RenderableMesh::~RenderableMesh()
	{

	}

	void RenderableMesh::Render()
	{	
		rgr::Camera* camera = m_ScenePtr->GetMainCamera(); // TODO: Check if camera can be made const
		rgr::Shader* shader = m_Material->GetShader(); // TODO: Check if shader can be made const

		const glm::mat4 modelMat4 = this->GetTransform().GetModelMatrix();

		m_Mesh->GetVertexArray()->Bind();

		// Bind the index buffer only if the mesh uses it
		if (m_Mesh->GetMeshType() == Mesh::MeshType::INDEXED)
			m_Mesh->GetIndexBuffer()->Bind();

		// Bind the material and set user-defined uniforms
		m_Material->Bind();
		m_Material->SetUniforms();

		glm::mat4 mvp;

		// Branch rendering pipeline depending on which space this object is in (2D or 3D)
		if (this->GetTransform().space == Transform::Space::SCREEN_2D)
		{	
			// NOTE: Skip view matrix for now as 2D camera transformations has not been implemented yet
			mvp = camera->GetOrthographic() * modelMat4;
		}
		else if (this->GetTransform().space == Transform::Space::WORLD_3D)
		{
			mvp = camera->GetPerspective() * camera->GetView() * modelMat4;

			// Obtain variables related to lighting
			const auto& lights = m_ScenePtr->GetLightsAround(this->GetTransform().GetPosition(), affectedByLightDist);
			const glm::vec3 viewPos = camera->GetTransform().GetPosition();

			// If this material is marked lit, set lighting uniforms
			if (m_Material->GetIsLit())
				rgr::ProcessLighting(shader, lights, viewPos, modelMat4, this->GetTransform().GetNormalMatrix());
		}

		// Set u_MVP matrix uniform that is mandatory for any shader
		shader->SetUniformMat4("u_MVP", true, mvp);

		// Check if uniforms were set with errors, and if so, 
		// draw the object bright pink to indicate errors
		const int shaderCallback = shader->GetUniformsCallback();
		if (shaderCallback != 0)
		{
			m_Material->Unbind();
			rgr::DrawErrorPlaceholder(mvp);
		}

		// Check if camera is set to draw the wireframes of objects, and if so,
		// draw them with some plain color
		if (camera->viewMode == Camera::ViewMode::WIREFRAME)
		{
			m_Material->Unbind();
			rgr::DrawWireframe(mvp);
		}

		// Do the actual OpenGL draw call depending on which type of mesh is used
		rgr::DrawMesh(m_Mesh);

		// Resetting the wireframe mode if it was set in rgr::DrawWireframe();
		if (camera->viewMode == rgr::Camera::ViewMode::WIREFRAME)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void RenderableMesh::DepthRender(const glm::mat4& lightSpaceMatrix)
	{
		rgr::Shader* depthMapShader = rgr::Shader::GetDepthMapShader();

		m_Mesh->GetVertexArray()->Bind();

		if (m_Mesh->GetMeshType() == Mesh::MeshType::INDEXED)
			m_Mesh->GetIndexBuffer()->Bind();

		depthMapShader->Bind();
		depthMapShader->SetUniformMat4("u_LightSpaceMatrix", false, lightSpaceMatrix);
		depthMapShader->SetUniformMat4("u_Model", false, GetTransform().GetModelMatrix());

		DrawMesh(m_Mesh);
	}

}

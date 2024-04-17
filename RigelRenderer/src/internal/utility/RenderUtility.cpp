#include "RenderUtility.hpp"
#include "glAbstraction/GlAbstraction.hpp"
#include "RigelRenderer.hpp"
#include "glm.hpp"
#include "glew.h"

#include "Logger.hpp"

#include <vector>
#include <string>

namespace rgr
{	
	static const size_t MAX_DIR_LIGHTS_ARRAY_SIZE = 8;
	static const size_t MAX_POINT_LIGHTS_ARRAY_SIZE = 100;
	static const size_t MAX_SPOT_LIGHTS_ARRAY_SIZE = 36;

	void DrawMesh(const Mesh* mesh)
	{
		if (mesh->GetMeshType() == rgr::Mesh::MeshType::INDEXED)
			glDrawElements(GL_TRIANGLES, mesh->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		else
			glDrawArrays(GL_TRIANGLES, 0, mesh->GetVertsCount());
	}

	static unsigned int depthMapFBO = 0;

	static void ProcessSingleDirLight(rgr::DirectionalLight* light, const Scene* scene)
	{
		const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

		const unsigned int depthMap = light->GetDepthMapHandle();

		if (depthMapFBO == 0)
		{
			// Generate FBO for rendering the depth map
			glGenFramebuffers(1, &depthMapFBO);

			// Bind buffer, configure it and attach the texture for it to render to
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		// Render the scene to the depth map
		const auto& objects = scene->GetObjectsInFrustrum();
		rgr::Shader* shader = rgr::Shader::GetDepthMapShader();

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		for (size_t i = 0; i < objects.size(); i++)
		{
			RenderableMesh* obj = static_cast<RenderableMesh*>(objects[i]);
			if (obj != nullptr)
			{
				const glm::mat4 model = obj->GetTransform().GetModelMatrix();
				rgr::Mesh* mesh = obj->GetMesh();

				mesh->GetVertexArray()->Bind();

				if (mesh->GetMeshType() == Mesh::MeshType::INDEXED)
					mesh->GetIndexBuffer()->Bind();

				shader->Bind();
				shader->SetUniformMat4("u_LightSpaceMatrix", false, light->GetLightSpaceViewProj());
				shader->SetUniformMat4("u_Model", false, model);

				DrawMesh(mesh);
			}
		}

		// Restore the viewport size and unbind the frame buffer
		rgr::ViewportSize size = rgr::GetViewportSize();
		glViewport(0, 0, size.width, size.height);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Render the depth map to a quad for debug
		rgr::Mesh* quad = rgr::Mesh::Get2DQuadMesh();
		rgr::Shader* testShader = rgr::Shader::GetDepthTestShader();

		quad->GetVertexArray()->Bind();
		quad->GetIndexBuffer()->Bind();

		testShader->Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		testShader->SetUniform1i("u_DepthMap", depthMap);

		DrawMesh(quad);
	}

	void ProcessShadowCasters(const Scene* scene)
	{
		rgr::Camera* camera = scene->GetMainCamera();
		const auto& lights = scene->GetLightsAround(camera->GetTransform().GetPosition(), camera->shadowsVisibilityDistance);

		for (size_t i = 0; i < lights.size(); i++)
		{
			rgr::Light* light = lights[i];
			if (static_cast<DirectionalLight*>(light) != nullptr)
			{
				ProcessSingleDirLight(static_cast<DirectionalLight*>(light), scene);
				return;
			}
		}
	}

	void ProcessLighting(rgr::Shader* shader, const std::vector<rgr::Light*>& lights,
		const glm::vec3 viewPos, const glm::mat4& model, const glm::mat3& normal)
	{
		size_t dirCount = 0;
		size_t pointCount = 0;
		size_t spotCount = 0;

		// Iterate through all the lights affecting this object and set corresponding uniforms
		for (rgr::Light* light : lights)
		{
			if (dynamic_cast<rgr::DirectionalLight*>(light) != nullptr)
			{
				if (dirCount > (MAX_DIR_LIGHTS_ARRAY_SIZE - 1)) continue;

				rgr::DirectionalLight* dirLight = dynamic_cast<rgr::DirectionalLight*>(light);

				std::string uName = "u_DirectionalLights[" + std::to_string(dirCount) + "].";
				shader->SetUniformVec3(uName + "color", dirLight->color);
				shader->SetUniform1f(uName + "intensity", dirLight->intensity);
				shader->SetUniformVec3(uName + "direction", dirLight->direction);

				dirCount++;
			}
			if (dynamic_cast<rgr::PointLight*>(light) != nullptr)
			{
				if (pointCount > (MAX_POINT_LIGHTS_ARRAY_SIZE - 1)) continue;

				rgr::PointLight* pointLight = dynamic_cast<rgr::PointLight*>(light);

				std::string uName = "u_PointLights[" + std::to_string(pointCount) + "].";
				shader->SetUniformVec3(uName + "color", pointLight->color);
				shader->SetUniformVec3(uName + "position", pointLight->GetTransform().GetPosition());
				shader->SetUniform1f(uName + "intensity", pointLight->intensity);
				shader->SetUniform1f(uName + "constant", pointLight->constant);
				shader->SetUniform1f(uName + "linear", pointLight->linear);
				shader->SetUniform1f(uName + "quadratic", pointLight->quadratic);

				pointCount++;
			}
			else if (dynamic_cast<rgr::SpotLight*>(light) != nullptr)
			{
				if (spotCount > (MAX_SPOT_LIGHTS_ARRAY_SIZE - 1)) continue;

				rgr::SpotLight* spotLight = dynamic_cast<rgr::SpotLight*>(light);

				std::string uName = "u_SpotLights[" + std::to_string(spotCount) + "].";
				shader->SetUniformVec3(uName + "color", spotLight->color);
				shader->SetUniformVec3(uName + "position", spotLight->GetTransform().GetPosition());
				shader->SetUniformVec3(uName + "direction", spotLight->direction);
				shader->SetUniform1f(uName + "intensity", spotLight->intensity);
				shader->SetUniform1f(uName + "constant", spotLight->constant);
				shader->SetUniform1f(uName + "linear", spotLight->linear);
				shader->SetUniform1f(uName + "quadratic", spotLight->quadratic);
				shader->SetUniform1f(uName + "cutOff", spotLight->cutOff);
				shader->SetUniform1f(uName + "outerCutOff", spotLight->outerCutOff);

				spotCount++;
			}
		}

		shader->SetUniformVec3("u_ViewPos", viewPos);
		shader->SetUniformMat4("u_Model", false, model);
		shader->SetUniformMat3("u_NormalMatrix", true, normal);

		shader->SetUniform1i("u_DirectionalLightsCount", dirCount);
		shader->SetUniform1i("u_PointLightsCount", pointCount);
		shader->SetUniform1i("u_SpotLightsCount", spotCount);
	}

	void DrawErrorPlaceholder(const glm::mat4& mvp)
	{
		rgr::Shader* phShader = rgr::Shader::GetPlainColorShader();

		/*
		If there were any errors, then unbind material with errors and use placeholder shader
		Placeholder shader will draw the object with bright pink color indicating an error
		*/

		phShader->Bind();
		phShader->SetUniformMat4("u_MVP", true, mvp);
		phShader->SetUniformVec4("u_Color", glm::vec4(0.96, 0.25, 0.89, 1.0));
	}

	void DrawWireframe(const glm::mat4& mvp)
	{
		rgr::Shader* phShader = rgr::Shader::GetPlainColorShader();

		phShader->Bind();
		phShader->SetUniformMat4("u_MVP", true, mvp);
		phShader->SetUniformVec4("u_Color", glm::vec4(0.0, 0.0, 0.0, 1.0));

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

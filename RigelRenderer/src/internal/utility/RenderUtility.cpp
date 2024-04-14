#include "glew.h"
#include "RenderUtility.hpp"
#include "glAbstraction/GlAbstraction.hpp"
#include "RigelRenderer.hpp"
#include "glm.hpp"

#include <vector>
#include <string>

namespace rgr
{	
	static const size_t MAX_DIR_LIGHTS_ARRAY_SIZE = 8;
	static const size_t MAX_POINT_LIGHTS_ARRAY_SIZE = 100;
	static const size_t MAX_SPOT_LIGHTS_ARRAY_SIZE = 36;

	static unsigned int ProcessSingleDirLight(rgr::DirectionalLight* light)
	{	
		// Generate FBO for rendering the depth map
		unsigned int depthMapFBO;
		glGenFramebuffers(1, &depthMapFBO);

		const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

		// Generate the texture to store the depth map
		unsigned int depthMap;
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Bind buffer, configure it and attach the texture for it to render to
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Configure light's point of view matrices
		const glm::mat4 proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 20.0f);

		// Create a view matrix that looks in the same direction as light
		const glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), -light->direction));
		const glm::vec3 up = glm::normalize(glm::cross(-light->direction, right));
		const glm::mat4 view = glm::lookAt(glm::vec3(0.0f), -light->direction, up);

		const glm::mat4 lightSpaceMatrix = proj * view;

		return depthMap;
	}

	void ProcessLighting(rgr::Shader* shader, const std::vector<rgr::Light*>& lights,
		const glm::vec3 viewPos, const glm::mat4& model, const glm::mat3& normal)
	{
		size_t dirCount = 0;
		size_t pointCount = 0;
		size_t spotCount = 0;

		// Iterate through all the lights affecting this object and setting corresponding uniforms
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

	void ProcessShadowCasters(const Scene* scene)
	{	
		rgr::Camera* camera = scene->GetMainCamera();
		const auto& lights = scene->GetLightsAround(camera->GetTransform().GetPosition(), camera->shadowsVisibilityDistance);
	}
}

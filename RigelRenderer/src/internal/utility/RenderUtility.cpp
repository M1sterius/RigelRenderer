#include "glew.h"
#include "RenderUtility.hpp"
#include "components/Shader.hpp"
#include "glAbstraction/GlAbstraction.hpp"
#include "Light.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"

#include <vector>
#include <string>

void rgr::ProcessLighting(rgr::Shader* shader, const std::vector<rgr::Light*>& lights, 
	const glm::vec3 viewPos, const glm::mat4& model, const glm::mat3& normal)
{	
	static const unsigned int MAX_DIR_LIGHTS_ARRAY_SIZE = 8;
	static const unsigned int MAX_POINT_LIGHTS_ARRAY_SIZE = 100;
	static const unsigned int MAX_SPOT_LIGHTS_ARRAY_SIZE = 36;

	unsigned int dirCount = 0;
	unsigned int pointCount = 0;
	unsigned int spotCount = 0;

	// Iterating through all the lights affecting this object and setting corresponding uniforms
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

void rgr::DrawErrorPlaceholder(const glm::mat4& mvp)
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

void rgr::DrawWireframe(const glm::mat4& mvp)
{
	rgr::Shader* phShader = rgr::Shader::GetPlainColorShader();

	phShader->Bind();
	phShader->SetUniformMat4("u_MVP", true, mvp);
	phShader->SetUniformVec4("u_Color", glm::vec4(0.0, 0.0, 0.0, 1.0));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

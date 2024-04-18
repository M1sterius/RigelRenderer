#include "Lights/DirectionalLight.hpp"
#include "glm.hpp"
#include "glew.h"

rgr::DirectionalLight::DirectionalLight(const glm::vec3& color, const float intensity, const glm::vec3& direction)
{
	this->color = color;
	this->intensity = intensity;

	this->direction = direction;

	// Generate 2D texture for depth map
	glGenTextures(1, &m_DepthMapHandle);
	glBindTexture(GL_TEXTURE_2D, m_DepthMapHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		m_DepthMapSize, m_DepthMapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);
}

rgr::DirectionalLight::~DirectionalLight()
{
	
}

const glm::mat4 rgr::DirectionalLight::GetLightSpaceView()
{
	constexpr glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	const glm::vec3 dir = glm::normalize(direction);
	const glm::vec3 pos = GetTransform().GetPosition();
	//const glm::vec3 right = glm::normalize(glm::cross(up, dir));
	const glm::mat4 view = glm::lookAt(pos, pos + dir, up);

	return view;
}

const glm::mat4 rgr::DirectionalLight::GetLightSpaceViewProj()
{
	static glm::mat4 proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, m_DepthProjNear, m_DepthProjFar);
	const glm::mat4 viewProj = proj * GetLightSpaceView();

	return viewProj;
}

void rgr::DirectionalLight::GenerateDepthMap()
{

}

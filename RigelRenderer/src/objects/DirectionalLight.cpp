#include "DirectionalLight.hpp"
#include "glm.hpp"

rgr::DirectionalLight::DirectionalLight(const glm::vec3& color, const float intensity, const glm::vec3& direction)
{
	this->color = color;
	this->intensity = intensity;

	this->direction = direction;
}

rgr::DirectionalLight::~DirectionalLight()
{

}

const glm::mat4 rgr::DirectionalLight::GetLightSpaceView()
{
	//const glm::mat4 view = glm::lookAt(glm::vec3(1.0) * 7.0f, glm::vec3(0.0), glm::vec3(0.0f, 1.0f, 0.0f));
	
	constexpr glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	const glm::vec3 dir = glm::normalize(direction);
	const glm::vec3 pos = GetTransform().GetPosition();
	//const glm::vec3 right = glm::normalize(glm::cross(up, dir));
	const glm::mat4 view = glm::lookAt(pos, pos + dir, up);

	return view;
}

const glm::mat4 rgr::DirectionalLight::GetLightSpaceViewProj()
{
	static glm::mat4 proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 20.0f);
	const glm::mat4 viewProj = proj * GetLightSpaceView();

	return viewProj;
}

void rgr::DirectionalLight::GenerateDepthMap()
{

}

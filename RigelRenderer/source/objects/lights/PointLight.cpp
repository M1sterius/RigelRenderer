#include "lights/PointLight.hpp"

rgr::PointLight::PointLight(const glm::vec3 color, const float intensity, const float constant, const float linear, const float quadratic)
    : constant(constant), linear(linear), quadratic(quadratic)
{
	this->color = color;
	this->intensity = intensity;
}

const glm::mat4 rgr::PointLight::GetLightSpaceView()
{
	return glm::mat4(0.0f);
}

const glm::mat4 rgr::PointLight::GetLightSpaceViewProj()
{
	return glm::mat4(0.0f);
}

void rgr::PointLight::GenerateDepthMap()
{

}

const float rgr::PointLight::GetLightRange()
{
    return 1000.0f;
}

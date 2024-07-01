#include "lights/SpotLight.hpp"

rgr::SpotLight::SpotLight(const glm::vec3 color, const float intensity, const glm::vec3& direction,
	const float cutOff, const float outerCutOff, const float constant, const float linear, const float quadratic)
    : direction(direction), cutOff(cutOff), outerCutOff(outerCutOff), constant(constant), linear(linear), quadratic(quadratic)
{
	this->color = color;
	this->intensity = intensity;
}

const glm::mat4 rgr::SpotLight::GetLightSpaceView()
{
	return glm::mat4(0.0f);
}

const glm::mat4 rgr::SpotLight::GetLightSpaceViewProj()
{
	return glm::mat4(0.0f);
}

void rgr::SpotLight::GenerateDepthMap()
{
	
}

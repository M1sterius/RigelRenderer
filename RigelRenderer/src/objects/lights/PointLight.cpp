#include "Lights/PointLight.hpp"

rgr::PointLight::PointLight(const glm::vec3 color, const float instensity, const float constant, const float linear, const float quadratic)
{
	this->color = color;
	this->intensity = instensity;

	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

rgr::PointLight::~PointLight()
{

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

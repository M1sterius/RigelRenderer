#include "PointLight.hpp"

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

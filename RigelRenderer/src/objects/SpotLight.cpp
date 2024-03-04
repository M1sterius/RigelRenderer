#include "SpotLight.hpp"

rgr::SpotLight::SpotLight(const glm::vec3 color, const float instensity, const glm::vec3& direction, 
	const float cutOff, const float outerCutOff, const float constant, const float linear, const float quadratic)
{
	this->color = color;
	this->intensity = instensity;

	this->direction = direction;
	this->cutOff = cutOff;
	this->outerCutOff = outerCutOff;

	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

rgr::SpotLight::~SpotLight()
{

}

#include "DirectionalLight.hpp"

rgr::DirectionalLight::DirectionalLight(const glm::vec3& color, const float intensity, const glm::vec3& direction)
{
	this->color = color;
	this->intensity = intensity;

	this->direction = direction;
}

rgr::DirectionalLight::~DirectionalLight()
{

}

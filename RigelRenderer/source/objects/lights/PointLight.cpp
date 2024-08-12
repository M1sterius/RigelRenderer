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
    const float Imax = fmax(fmax(color.x, color.y), color.z) * intensity;
    const float sq = sqrtf(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * Imax));
    return (-linear + sq) / (2 * quadratic);
}

glm::mat4 rgr::PointLight::GetLightVolumeModelMatrix()
{
    const float scale = GetLightRange() / 2;

    auto model = glm::mat4(1.0f);

    model = glm::translate(model, GetTransform().GetPosition());
    model *= glm::mat4_cast(glm::quat(glm::vec3(0,0, 0)));
    model = glm::scale(model, glm::vec3(scale));

    return model;
}

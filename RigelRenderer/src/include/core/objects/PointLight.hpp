#pragma once

#include "Light.hpp"
#include "vec3.hpp"

namespace rgr
{
	class PointLight : rgr::Light
	{
	public:
		float constant;
		float linear;
		float quadratic;

		PointLight(const glm::vec3 color, const float instensity, const float constant, const float linear, const float quadratic);
		~PointLight() override;
	};
}


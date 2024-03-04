#pragma once

#include "objects/Light.hpp"
#include "vec3.hpp"

namespace rgr
{
	class SpotLight : rgr::Light
	{
	public:
		glm::vec3 direction;
		float cutOff;
		float outerCutOff;

		float constant;
		float linear;
		float quadratic;

		SpotLight(const glm::vec3 color, const float instensity, const glm::vec3& direction, 
			const float cutOff, const float outerCutOff, const float constant, const float linear, const float quadratic);
		~SpotLight() override;
	};
}


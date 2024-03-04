#pragma once

#include "objects/Light.hpp"
#include "vec3.hpp"

namespace rgr
{
	class DirectionalLight : public rgr::Light
	{
	public:
		glm::vec3 direction;

		DirectionalLight(const glm::vec3& color, const float intensity, const glm::vec3& direction);
		~DirectionalLight() override;
	};
}


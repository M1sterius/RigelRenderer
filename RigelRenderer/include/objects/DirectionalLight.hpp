#pragma once

#include "objects/Light.hpp"
#include "glm.hpp"

namespace rgr
{
	class DirectionalLight : public rgr::Light
	{
	public:
		glm::vec3 direction;

		DirectionalLight(const glm::vec3& color, const float intensity, const glm::vec3& direction);
		~DirectionalLight() override;

		const glm::mat4 GetLightSpaceView() override;
		const glm::mat4 GetLightSpaceViewProj() override;
	};
}


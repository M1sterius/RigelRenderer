#pragma once

#include "objects/lights/Light.hpp"

namespace rgr
{
	class PointLight : public rgr::Light
	{
	public:
		float constant;
		float linear;
		float quadratic;

		PointLight(const glm::vec3 color, const float intensity, const float constant, const float linear, const float quadratic);
		~PointLight() override = default;

        const float GetLightRange() override;
        glm::mat4 GetLightVolumeModelMatrix();
		const glm::mat4 GetLightSpaceView() override;
		const glm::mat4 GetLightSpaceViewProj() override;
	INTERNAL:
		void GenerateDepthMap() override;
	};
}


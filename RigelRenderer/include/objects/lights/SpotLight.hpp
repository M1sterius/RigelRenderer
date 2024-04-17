#pragma once

#include "objects/Lights/Light.hpp"

namespace rgr
{
	class SpotLight : public rgr::Light
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
		virtual ~SpotLight();

		const glm::mat4 GetLightSpaceView() override;
		const glm::mat4 GetLightSpaceViewProj() override;
	INTERNAL:
		void GenerateDepthMap() override;
	};
}


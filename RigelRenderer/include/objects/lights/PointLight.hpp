#pragma once

#include "objects/Lights/Light.hpp"

namespace rgr
{
	class PointLight : public rgr::Light
	{
	public:
		float constant;
		float linear;
		float quadratic;

		PointLight(const glm::vec3 color, const float instensity, const float constant, const float linear, const float quadratic);
		virtual ~PointLight();

		const glm::mat4 GetLightSpaceView() override;
		const glm::mat4 GetLightSpaceViewProj() override;
	INTERNAL:
		void GenerateDepthMap(const unsigned int depthMapFBO) override;
	};
}


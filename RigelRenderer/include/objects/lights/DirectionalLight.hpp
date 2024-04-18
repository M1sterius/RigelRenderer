#pragma once

#include "objects/Lights/Light.hpp"

namespace rgr
{
	class DirectionalLight : public rgr::Light
	{
	private:
		const size_t m_DepthMapSize = 1024;
		const float m_DepthProjNear = 0.1f;
		const float m_DepthProjFar = 20.0f;
	public:
		glm::vec3 direction;

		DirectionalLight(const glm::vec3& color, const float intensity, const glm::vec3& direction);
		virtual ~DirectionalLight();

		const glm::mat4 GetLightSpaceView() override;
		const glm::mat4 GetLightSpaceViewProj() override;
	INTERNAL:
		void GenerateDepthMap(const unsigned int depthMapFBO) override;
	};
}


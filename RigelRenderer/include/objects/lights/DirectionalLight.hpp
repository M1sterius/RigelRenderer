#pragma once

#include "objects/lights/Light.hpp"

namespace rgr
{
	class DirectionalLight : public rgr::Light
	{
	private:
		const float m_DepthProjNear = 0.1f;
		const float m_DepthProjFar = 20.0f;

        glm::mat4 m_ProjMatrix;

        void CalcProjMatrix();
	public:
		glm::vec3 direction;

		DirectionalLight(const glm::vec3& color, const float intensity, const glm::vec3& direction);
		~DirectionalLight() override = default;

		const glm::mat4 GetLightSpaceView() override;
		const glm::mat4 GetLightSpaceViewProj() override;
	INTERNAL:
		void GenerateDepthMap() override;
	};
}

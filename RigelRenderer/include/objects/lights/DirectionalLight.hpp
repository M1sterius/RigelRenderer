#pragma once

#include "objects/lights/Light.hpp"

namespace rgr
{
	class DirectionalLight : public rgr::Light
	{
	private:
		float m_DepthProjNear = 0.1f;
		float m_DepthProjFar = 20.0f;
        float m_ProjWidth = 20.0f;
        float m_ProjHeight = 20.0f;

        glm::mat4 m_ProjMatrix;

        void CalcProjMatrix();
	public:
		glm::vec3 direction;

		DirectionalLight(const glm::vec3& color, const float intensity, const glm::vec3& direction);
		~DirectionalLight() override = default;

        const float GetLightRange() override;
		const glm::mat4 GetLightSpaceView() override;
		const glm::mat4 GetLightSpaceViewProj() override;

        void SetDepthProjectionClip(const float near, const float far, const float width, const float height);
        inline glm::vec4 GetDepthProjectionClip() const { return {m_DepthProjNear, m_DepthProjFar, m_ProjWidth, m_ProjHeight}; }
	INTERNAL:
		void GenerateDepthMap() override;
        glm::vec2 atlasOffset;
	};
}

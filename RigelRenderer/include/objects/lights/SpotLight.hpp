#pragma once

#include "objects/lights/Light.hpp"

namespace rgr
{
	class SpotLight : public rgr::Light
	{
    private:
        float m_DepthProjNear = 0.1f;
        float m_DepthProjFar = 20.0f;

        float m_CutOff;

        glm::mat4 m_ProjMatrix;

        void CalcProjMatrix();
	public:
		glm::vec3 direction;
		float outerCutOff;

		float constant;
		float linear;
		float quadratic;

		SpotLight(const glm::vec3 color, const float intensity, const glm::vec3& direction,
			const float cutOff, const float outerCutOff, const float constant, const float linear, const float quadratic);
		~SpotLight() override = default;

        const float GetLightRange() override;
        glm::mat4 GetLightVolumeModelMatrix();
		const glm::mat4 GetLightSpaceView() override;
		const glm::mat4 GetLightSpaceViewProj() override;

        void SetDepthProjectionClipPlanes(const float near, const float far);
        void SetCutOff(const float cutOff);

        inline glm::vec2 GetDepthProjectionClip() const { return {m_DepthProjNear, m_DepthProjFar}; }
        inline float GetCutOff() const { return m_CutOff; }
	INTERNAL:
		void GenerateDepthMap() override;
        glm::vec2 atlasOffset;
	};
}


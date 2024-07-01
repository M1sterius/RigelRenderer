#include "lights/SpotLight.hpp"
#include "renderable/RenderableMesh.hpp"
#include "RigelRenderer.hpp"
#include "glm.hpp"

rgr::SpotLight::SpotLight(const glm::vec3 color, const float intensity, const glm::vec3& direction,
	const float cutOff, const float outerCutOff, const float constant, const float linear, const float quadratic)
    : direction(direction), cutOff(cutOff), outerCutOff(outerCutOff), constant(constant), linear(linear), quadratic(quadratic)
{
	this->color = color;
	this->intensity = intensity;
}

const glm::mat4 rgr::SpotLight::GetLightSpaceView()
{
    constexpr glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    const glm::vec3 dir = glm::normalize(direction);
    const glm::vec3 pos = GetTransform().GetPosition();
    const glm::mat4 view = glm::lookAt(pos, pos + dir, up);

    return view;
}

const glm::mat4 rgr::SpotLight::GetLightSpaceViewProj()
{
    static const glm::mat4 proj = glm::perspective(cutOff * 2.0f, 1.0f, m_DepthProjNear, m_DepthProjFar);
    const glm::mat4 viewProj = proj * this->GetLightSpaceView();

    return viewProj;
}

void rgr::SpotLight::GenerateDepthMap()
{
    const auto& renderables = m_ScenePtr->GetRenderablesInFrustum();

    for (auto i : renderables)
    {
        auto renderable = dynamic_cast<rgr::RenderableMesh*>(i);

        if (renderable == nullptr) continue;
        if (!renderable->shadowCaster) continue;

        renderable->RenderDepth(GetLightSpaceViewProj());
    }
}

#include "lights/DirectionalLight.hpp"
#include "renderable/RenderableMesh.hpp"
#include "RigelRenderer.hpp"
#include "glm.hpp"

rgr::DirectionalLight::DirectionalLight(const glm::vec3& color, const float intensity, const glm::vec3& direction)
    : direction(direction)
{
	this->color = color;
	this->intensity = intensity;

    CalcProjMatrix();
}

const float rgr::DirectionalLight::GetLightRange()
{
    return INFINITY;
}

const glm::mat4 rgr::DirectionalLight::GetLightSpaceView()
{
	constexpr glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	const glm::vec3 dir = glm::normalize(direction);
	const glm::vec3 pos = GetTransform().GetPosition();
	const glm::mat4 view = glm::lookAt(pos, pos + dir, up);

	return view;
}

const glm::mat4 rgr::DirectionalLight::GetLightSpaceViewProj()
{
	const glm::mat4 viewProj = m_ProjMatrix * this->GetLightSpaceView();

	return viewProj;
}

void rgr::DirectionalLight::SetDepthProjectionClip(const float near, const float far, const float width, const float height)
{

}


void rgr::DirectionalLight::GenerateDepthMap()
{
	const auto& renderables = m_ScenePtr->GetRenderablesInFrustum();

    const glm::mat4 lightSpaceViewProj = GetLightSpaceViewProj();

	for (auto i : renderables)
	{
		auto renderable = dynamic_cast<rgr::RenderableMesh*>(i);

		if (renderable == nullptr) continue;
		if (!renderable->shadowCaster) continue;

		renderable->RenderDepth(lightSpaceViewProj);
	}
}

void rgr::DirectionalLight::CalcProjMatrix()
{
    m_ProjMatrix = glm::ortho(-(m_ProjWidth / 2), (m_ProjWidth / 2), -(m_ProjHeight / 2), (m_ProjHeight / 2),
                              m_DepthProjNear, m_DepthProjFar);
}
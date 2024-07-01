#include "lights/DirectionalLight.hpp"
#include "renderable/RenderableMesh.hpp"
#include "RigelRenderer.hpp"
#include "glm.hpp"
#include "glew.h"

rgr::DirectionalLight::DirectionalLight(const glm::vec3& color, const float intensity, const glm::vec3& direction)
{
	this->color = color;
	this->intensity = intensity;

	this->direction = direction;
}

rgr::DirectionalLight::~DirectionalLight()
{
	
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
	static glm::mat4 proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, m_DepthProjNear, m_DepthProjFar);
	const glm::mat4 viewProj = proj * GetLightSpaceView();

	return viewProj;
}

void rgr::DirectionalLight::GenerateDepthMap()
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

#include "Renderer.hpp"
#include "GBuffer.hpp"
#include "lights/DirectionalLight.hpp"
#include "renderable/RenderableMesh.hpp"
#include "Scene.hpp"

#include "glew.h"

namespace rgr
{	
	static void DrawDebugQuad(const unsigned int textureHandle)
	{
		rgr::Mesh* quad = rgr::Mesh::Get2DQuadMesh();
		rgr::Shader* testShader = rgr::Shader::GetDepthTestShader();

		quad->GetVertexArray()->Bind();
		quad->GetIndexBuffer()->Bind();

		testShader->Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureHandle);
		testShader->SetUniform1i("u_DepthMap", textureHandle);

		quad->Draw();
	}

	void Renderer::GenerateDepthMapsForLightSources(const Scene* scene)
	{
		static unsigned int depthMapFBO = 0;

		// Pre-setup depth map framebuffer for future depth map generations only if it hasn't been set up yet
		if (depthMapFBO == 0)
		{
			// Note that attaching a depth map is skipped, because one will later be attached for each light individually
			glGenFramebuffers(1, &depthMapFBO);
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		rgr::Camera* camera = scene->GetMainCamera();
		const auto& lights = scene->GetLightsAround(camera->GetTransform().GetPosition(), camera->shadowsVisibilityDistance);

		for (size_t i = 0; i < lights.size(); i++)
		{
			rgr::Light* light = lights[i];

			if (!light->castShadows) continue;

			light->GenerateDepthMap(depthMapFBO);

			//Render the depth map of the last directional light to a quad for debug
			if (static_cast<DirectionalLight*>(light) != nullptr)
			{
				DrawDebugQuad(light->GetDepthMapHandle());
			}
		}
	}

	void Renderer::DoGeometryPass(const Scene* scene, const GBuffer* gBuffer)
	{
		const auto& renderables = scene->GetRenderablesInFrustrum();
		const rgr::Shader* shader = rgr::Shader::GetGeometryPassShader();
		const glm::mat4 viewProj = scene->GetMainCamera()->GetPerspective() * scene->GetMainCamera()->GetView();

		gBuffer->Bind();
		gBuffer->Clear();

		shader->Bind();

		for (size_t i = 0; i < renderables.size(); i++)
		{
			rgr::RenderableMesh* renderable = dynamic_cast<rgr::RenderableMesh*>(renderables[i]);

			if (renderable == nullptr) continue;

			renderable->RenderGeometry(shader, viewProj);
		}
	}

	void Renderer::DoLightingPass(const Scene* scene, const GBuffer* gBuffer)
	{

	}

}
#include "Renderer.hpp"
#include "GBuffer.hpp"
#include "lights/DirectionalLight.hpp"
#include "renderable/RenderableMesh.hpp"
#include "Scene.hpp"

#include "glew.h"

#include <iostream>

namespace rgr
{	
	static void DrawDebugQuad(const unsigned int textureHandle)
	{
		rgr::Mesh* quad = rgr::Mesh::Get2DQuadMesh();
		rgr::Shader* testShader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::TEXTURE_TEST);

		testShader->Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureHandle);
		testShader->SetUniform1i("u_Texture", 0);

		quad->Draw();

		testShader->Unbind();
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

		for (auto light : lights)
		{
            if (!light->castShadows) continue;

			light->GenerateDepthMap(depthMapFBO);

			//Render the depth map of the last directional light to a quad for debug
//			if (static_cast<DirectionalLight*>(light) != nullptr)
//			{
//				DrawDebugQuad(light->GetDepthMapHandle());
//			}
		}
	}

	void Renderer::DoGeometryPass(const Scene* scene, const GBuffer* gBuffer)
	{
		const auto& renderables = scene->GetRenderablesInFrustrum();
		rgr::Shader* shader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::GEOMETRY_PASS);
		const glm::mat4 viewProj = scene->GetMainCamera()->GetPerspective() * scene->GetMainCamera()->GetView();

		gBuffer->Bind();
		gBuffer->Clear();

		shader->Bind();

		for (auto i : renderables)
		{
			auto renderable = dynamic_cast<rgr::RenderableMesh*>(i);
			if (renderable == nullptr) continue;
			renderable->RenderGeometry(shader, viewProj);
		}

		gBuffer->Unbind();

		// Draw a texture from gBuffer to a quad for debug
		DrawDebugQuad(gBuffer->GetColorTexture());
	}

	void Renderer::DoLightingPass(const Scene* scene, const GBuffer* gBuffer)
	{

	}

	void Renderer::DoForwardPass(const Scene* scene)
	{

	}
}

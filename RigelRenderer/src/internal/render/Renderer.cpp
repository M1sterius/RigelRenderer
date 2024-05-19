#include "Renderer.hpp"
#include "GBuffer.hpp"
#include "lights/DirectionalLight.hpp"
#include "renderable/RenderableMesh.hpp"
#include "Scene.hpp"

#include "glew.h"

#include <iostream>

namespace rgr
{
    unsigned int Renderer::m_DepthMapFBOHandle = 0;

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

    void Renderer::InitializeDepthMapFBO()
    {
        // Note that attaching a depth map texture is skipped, because one will later be attached for each light individually
        glGenFramebuffers(1, &m_DepthMapFBOHandle);
        glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBOHandle);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

	void Renderer::GenerateDepthMapsForLightSources(const Scene* scene)
	{
		// Pre-setup depth map framebuffer for future depth map generations only if it hasn't been set up yet
		if (m_DepthMapFBOHandle == 0)
            InitializeDepthMapFBO();

		rgr::Camera* camera = scene->GetMainCamera();
		const auto& lights = scene->GetLightsAround(camera->GetTransform().GetPosition(), camera->shadowsVisibilityDistance);

		for (auto light : lights)
		{
            if (!light->castShadows) continue;
			light->GenerateDepthMap(m_DepthMapFBOHandle);
		}
	}

	void Renderer::DoGeometryPass(const Scene* scene, const GBuffer* gBuffer)
	{
		const auto& renderables = scene->GetRenderablesInFrustum();
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
		//DrawDebugQuad(gBuffer->GetColorTexture());
	}

    void Renderer::SetDirectionalLightUniforms(const DirectionalLight* light, Shader* shader, const size_t lightIndex)
    {
        std::string u_name = "u_DirectionalLights[" + std::to_string(lightIndex) + "].";
        shader->SetUniformVec3(u_name + "color", light->color);
        shader->SetUniform1f(u_name + "intensity", light->intensity);
        shader->SetUniformVec3(u_name + "direction", light->direction);
    }

	void Renderer::DoLightingPass(const Scene* scene, const GBuffer* gBuffer)
	{
        rgr::Camera* camera = scene->GetMainCamera();
        const auto& lights = scene->GetLightsAround(camera->GetTransform().GetPosition(),camera->shadowsVisibilityDistance);
        rgr::Shader* shader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::LIGHTING_PASS);
        rgr::Mesh* quad = rgr::Mesh::Get2DQuadMesh();

        gBuffer->Clear();
        shader->Bind();

        shader->SetUniform1i("g_Position", 0);
        shader->SetUniform1i("g_Normal", 1);
        shader->SetUniform1i("g_AlbedoSpec", 2);
        shader->SetUniformVec3("u_ViewPos", camera->GetTransform().GetPosition());

        gBuffer->BindPositionTexture();
        gBuffer->BindNormalTexture();
        gBuffer->BindColorTexture();

        size_t dirCount = 0;

        for (auto light: lights)
        {
            if (auto dirLight = dynamic_cast<DirectionalLight*>(light))
            {
                if (dirCount > (MAX_DIR_LIGHTS_COUNT - 1)) continue;
                SetDirectionalLightUniforms(dirLight, shader, dirCount);
                dirCount++;
            }
        }

        shader->SetUniform1i("u_DirLightsCount", dirCount);

        quad->Draw();

        glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer->GetFBOHandle());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, gBuffer->GetBufferWidth(), gBuffer->GetBufferHeight(),
                          0, 0, gBuffer->GetBufferWidth(), gBuffer->GetBufferHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Renderer::DoForwardPass(const Scene* scene)
	{

	}


}

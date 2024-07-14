#include "RenderHandler.hpp"

#include "GBuffer.hpp"
#include "RigelRenderer.hpp"

#include "glad.h"

#include <vector>
#include <iostream>

namespace rgr
{

    RenderHandler::RenderHandler(rgr::Scene* scene)
    {
        m_Scene = scene;
        m_GBuffer = std::make_unique<rgr::GBuffer>(rgr::GetViewportSize().width, rgr::GetViewportSize().height);

        InitializeDepthAtlases();
        InitializeDepthMapFBOs();

        // Set uniform variables that are persistent among bindings
        auto lightingPassShader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::LIGHTING_PASS);
        lightingPassShader->Bind();
        lightingPassShader->SetUniform1i("g_Position", 0);
        lightingPassShader->SetUniform1i("g_Normal", 1);
        lightingPassShader->SetUniform1i("g_AlbedoSpec", 2);
        lightingPassShader->SetUniform1i("u_DirLightsShadowAtlas", 3);
        lightingPassShader->SetUniform1i("u_SpotLightsShadowAtlas", 4);
        lightingPassShader->Unbind();
    }

    RenderHandler::~RenderHandler()
    {
        DeleteDepthMapFBOs();
    }

    void RenderHandler::GenerateDepthMaps()
    {
        auto camera = m_Scene->GetMainCamera();
        const auto& lights = m_Scene->GetLightsAround(camera->GetTransform().GetPosition(), camera->shadowsVisibilityDistance);

        glCullFace(GL_FRONT);
        ClearDepthAtlases();

        size_t dirLightsCount = 0;
        size_t spotLightsCount = 0;

        for (auto light : lights)
        {
            if (!light->castShadows) continue;

            if (auto dirLight = dynamic_cast<rgr::DirectionalLight*>(light))
            {
                const size_t dirMapSize = rgr::DirectionalLight::depthMapSize;
                const auto x_pos = static_cast<size_t>(dirLightsCount % 4) * dirMapSize;
                const auto y_pos = static_cast<size_t>(dirLightsCount / 4) * dirMapSize;

                glBindFramebuffer(GL_FRAMEBUFFER, m_DirLightsFBOHandle);
                glViewport(x_pos, y_pos, dirMapSize, dirMapSize);

                dirLight->GenerateDepthMap();

                dirLightsCount++;
            }
            else if (auto spotLight = dynamic_cast<rgr::SpotLight*>(light))
            {
                const size_t spotMapSize = rgr::SpotLight::depthMapSize;
                const auto x_pos = static_cast<size_t>(spotLightsCount % 8) * spotMapSize;
                const auto y_pos = static_cast<size_t>(spotLightsCount / 8) * spotMapSize;

                glBindFramebuffer(GL_FRAMEBUFFER, m_SpotLightsFBOHandle);
                glViewport(x_pos, y_pos, spotMapSize, spotMapSize);

                spotLight->GenerateDepthMap();

                spotLightsCount++;
            }
        }

        rgr::ViewportSize size = rgr::GetViewportSize();
        glViewport(0, 0, size.width, size.height);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

//        glViewport(0, 0, 900, 900);
//        rgr::Mesh* quad = rgr::Mesh::Get2DQuadMesh();
//        rgr::Shader* shader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::TEXTURE_TEST);
//
//        shader->Bind();
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, m_SpotLightsDepthAtlasHandle);
//        shader->SetUniform1i("u_Texture", m_SpotLightsDepthAtlasHandle);
//
//        quad->Draw();

        glCullFace(GL_BACK);
    }

    void RenderHandler::DoGeometryPass()
    {
        const auto& renderables = m_Scene->GetRenderablesInFrustum();
        auto shader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::GEOMETRY_PASS);
        const glm::mat4 viewProj = m_Scene->GetMainCamera()->GetPerspective() * m_Scene->GetMainCamera()->GetView();

        m_GBuffer->Bind();
        m_GBuffer->ClearColorDepthBufferBit();

        shader->Bind();

        for (auto i : renderables)
        {
            auto renderable = dynamic_cast<rgr::RenderableMesh*>(i);
            if (renderable == nullptr) continue;
            renderable->RenderGeometry(shader, viewProj);
        }

        m_GBuffer->Unbind();
    }

    void RenderHandler::SetDirLightUniforms(rgr::DirectionalLight* light, rgr::Shader* shader, const size_t lightIndex)
    {
        std::string u_name = "u_DirectionalLights[" + std::to_string(lightIndex) + "].";
        shader->SetUniformVec3(u_name + "color", light->color);
        shader->SetUniform1f(u_name + "intensity", light->intensity);
        shader->SetUniformVec3(u_name + "direction", light->direction);
        shader->SetUniformBool(u_name + "smoothShadows", light->smoothShadows);
        shader->SetUniformMat4(u_name + "lightSpaceViewProj", false, light->GetLightSpaceViewProj());
    }

    void RenderHandler::SetSpotLightUniforms(rgr::SpotLight* light, rgr::Shader* shader, const size_t lightIndex)
    {
        std::string u_name = "u_SpotLights[" + std::to_string(lightIndex) + "].";
        shader->SetUniformVec3(u_name + "color", light->color);
        shader->SetUniform1f(u_name + "intensity", light->intensity);
        shader->SetUniformVec3(u_name + "position", light->GetTransform().GetPosition());
        shader->SetUniformVec3(u_name + "direction", light->direction);
        shader->SetUniform1f(u_name + "cutOff", light->GetCutOff());
        shader->SetUniform1f(u_name + "outerCutOff", light->outerCutOff);
        shader->SetUniform1f(u_name + "constant", light->constant);
        shader->SetUniform1f(u_name + "linear", light->linear);
        shader->SetUniform1f(u_name + "quadratic", light->quadratic);
        shader->SetUniformBool(u_name + "smoothShadows", light->smoothShadows);
        shader->SetUniformMat4(u_name + "lightSpaceViewProj", false, light->GetLightSpaceViewProj());
    }

    void RenderHandler::SetPointLightUniforms(rgr::PointLight* light, rgr::Shader* shader, const size_t lightIndex)
    {

    }

    void RenderHandler::DoLightingPass()
    {
        auto camera = m_Scene->GetMainCamera();
        auto shader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::LIGHTING_PASS);
        auto quad = rgr::Mesh::Get2DQuadMesh();
        const auto& lights = m_Scene->GetLightsAround(camera->GetTransform().GetPosition(),camera->shadowsVisibilityDistance);

        m_GBuffer->ClearColorDepthBufferBit();
        shader->Bind();

        m_GBuffer->BindPositionTexture();
        m_GBuffer->BindNormalTexture();
        m_GBuffer->BindColorTexture();

        m_DirLightsDepthAtlas->Bind(3);
        m_SpotLightsDepthAtlas->Bind(4);

        size_t dirCount = 0;
        size_t spotCount = 0;

        for (auto light: lights)
        {
            if (auto dirLight = dynamic_cast<DirectionalLight*>(light))
            {
                if (dirCount > (MAX_DIR_LIGHTS_COUNT - 1)) continue;
                SetDirLightUniforms(dirLight, shader, dirCount);
                dirCount++;
            }
            else if (auto spotLight = dynamic_cast<SpotLight*>(light))
            {
                if (spotCount > (MAX_SPOT_LIGHTS_COUNT - 1)) continue;
                SetSpotLightUniforms(spotLight, shader, spotCount);
                spotCount++;
            }
        }

        shader->SetUniformVec3("u_ViewPos", camera->GetTransform().GetPosition());
        shader->SetUniform1i("u_DirLightsCount", dirCount);
        shader->SetUniform1i("u_SpotLightsCount", spotCount);

        quad->Draw();

        BlitDeferredFBO();
    }

    void RenderHandler::DoForwardPass()
    {

    }

    void RenderHandler::DeleteDepthMapFBOs()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &m_DirLightsFBOHandle);
        glDeleteFramebuffers(1, &m_SpotLightsFBOHandle);
    }

    void RenderHandler::BlitDeferredFBO()
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GBuffer->GetFBOHandle());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, m_GBuffer->GetBufferWidth(), m_GBuffer->GetBufferHeight(),
                          0, 0, m_GBuffer->GetBufferWidth(), m_GBuffer->GetBufferHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void RenderHandler::InitializeDepthAtlases()
    {
        constexpr float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

        // Directional lights atlas
        static const size_t DIR_LIGHTS_ATLAS_SIZE = rgr::DirectionalLight::depthMapSize * 3; // total 9 maps
        m_DirLightsDepthAtlas = std::make_unique<rgr::Texture>(DIR_LIGHTS_ATLAS_SIZE, DIR_LIGHTS_ATLAS_SIZE, rgr::Texture::TYPE::DEPTH_COMPONENT);
        m_DirLightsDepthAtlas->SetFilter(rgr::Texture::FILTER::NEAREST, rgr::Texture::FILTER::NEAREST);
        m_DirLightsDepthAtlas->SetWrap(rgr::Texture::WRAP::CLAMP_TO_BORDER, rgr::Texture::WRAP::CLAMP_TO_BORDER);
        m_DirLightsDepthAtlas->SetBorderColor(borderColor);

        // Spotlights atlas
        static const size_t SPOT_LIGHTS_ATLAS_SIZE = rgr::SpotLight::depthMapSize * 8; // total 64 maps
        m_SpotLightsDepthAtlas = std::make_unique<rgr::Texture>(SPOT_LIGHTS_ATLAS_SIZE, SPOT_LIGHTS_ATLAS_SIZE, rgr::Texture::TYPE::DEPTH_COMPONENT);
    }

    void RenderHandler::InitializeDepthMapFBOs()
    {
        // Directional lights FBO
        glGenFramebuffers(1, &m_DirLightsFBOHandle);
        glBindFramebuffer(GL_FRAMEBUFFER, m_DirLightsFBOHandle);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DirLightsDepthAtlas->GetHandle(), 0);

        // Spotlights FBO
        glGenFramebuffers(1, &m_SpotLightsFBOHandle);
        glBindFramebuffer(GL_FRAMEBUFFER, m_SpotLightsFBOHandle);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_SpotLightsDepthAtlas->GetHandle(), 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void RenderHandler::ClearDepthAtlases() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_DirLightsFBOHandle);
        glClear(GL_DEPTH_BUFFER_BIT);
        glBindFramebuffer(GL_FRAMEBUFFER, m_SpotLightsFBOHandle);
        glClear(GL_DEPTH_BUFFER_BIT);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

//glViewport(0, 0, 900, 900);
//rgr::Mesh* quad = rgr::Mesh::Get2DQuadMesh();
//rgr::Shader* shader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::TEXTURE_TEST);
//
//shader->Bind();
//glActiveTexture(GL_TEXTURE0);
//glBindTexture(GL_TEXTURE_2D, m_SpotLightsDepthAtlasHandle);
//shader->SetUniform1i("u_Texture", m_SpotLightsDepthAtlasHandle);
//
//quad->Draw();
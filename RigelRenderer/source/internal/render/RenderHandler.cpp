#include "RenderHandler.hpp"

#include "GBuffer.hpp"
#include "RigelRenderer.hpp"
#include "mesh/Mesh.hpp"

#include "glad.h"

#include <vector>
#include <iostream>

namespace rgr
{

    RenderHandler::RenderHandler()
        : m_Scene(nullptr)
    {
        m_GBuffer = std::make_unique<rgr::GBuffer>(rgr::Core::GetWindowSize().x, rgr::Core::GetWindowSize().y);

        InitializeDepthAtlases();
        InitializeDepthMapFBOs();
        SetShadersConstantUniforms();
    }

    void RenderHandler::SetScene(rgr::Scene* scene)
    {
        m_Scene = scene;
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
        glEnable(GL_DEPTH_TEST);
        ClearDepthAtlases();

        size_t dirLightsCount = 0;
        size_t spotLightsCount = 0;

        for (const auto& light : lights)
        {
            if (!light->castShadows) continue;

            if (auto dirLight = std::dynamic_pointer_cast<rgr::DirectionalLight>(light))
            {
                const auto x_pos = static_cast<size_t>(dirLightsCount % 4) * DIR_LIGHT_SHADOW_MAP_SIZE;
                const auto y_pos = static_cast<size_t>(dirLightsCount / 4) * DIR_LIGHT_SHADOW_MAP_SIZE;

                glBindFramebuffer(GL_FRAMEBUFFER, m_DirLightsFBOHandle);
                glViewport(static_cast<int>(x_pos), static_cast<int>(y_pos), DIR_LIGHT_SHADOW_MAP_SIZE, DIR_LIGHT_SHADOW_MAP_SIZE);

                dirLight->GenerateDepthMap();
                dirLight->atlasOffset = glm::vec2(x_pos / DIR_LIGHT_ATLAS_SIZE, y_pos / DIR_LIGHT_ATLAS_SIZE);

                dirLightsCount++;
            }
            else if (auto spotLight = std::dynamic_pointer_cast<rgr::SpotLight>(light))
            {
                const size_t spotMapSize = rgr::SpotLight::depthMapSize;
                const auto x_pos = static_cast<size_t>(spotLightsCount % 8) * spotMapSize;
                const auto y_pos = static_cast<size_t>(spotLightsCount / 8) * spotMapSize;

                glBindFramebuffer(GL_FRAMEBUFFER, m_SpotLightsFBOHandle);
                glViewport(static_cast<int>(x_pos), static_cast<int>(y_pos), spotMapSize, spotMapSize);

                spotLight->GenerateDepthMap();

                spotLightsCount++;
            }
        }

        const glm::vec2 size = rgr::Core::GetWindowSize();
        glViewport(0, 0, static_cast<int>(size.x), static_cast<int>(size.y));
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glCullFace(GL_BACK);
        glDisable(GL_DEPTH_TEST);
    }

    void RenderHandler::DoGeometryPass()
    {
        const auto& renderables = m_Scene->GetRenderablesInFrustum();
        const auto& shader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::GEOMETRY_PASS);
        const glm::mat4 viewProj = m_Scene->GetMainCamera()->GetPerspective() * m_Scene->GetMainCamera()->GetView();

        glEnable(GL_DEPTH_TEST);

        m_GBuffer->Bind();
        m_GBuffer->ClearColorDepthBufferBit();

        shader.Bind();

        for (const auto& i : renderables)
        {
            auto renderable = std::dynamic_pointer_cast<rgr::RenderableModel>(i);
            if (renderable == nullptr) continue;
            renderable->RenderGeometry(shader, viewProj);
        }

        m_GBuffer->Unbind();

        glDisable(GL_DEPTH_TEST);
    }

    void RenderHandler::SetDirLightCommonUniforms(const std::shared_ptr<DirectionalLight>& light, const Shader& shader)
    {
        shader.SetUniformVec3("u_ViewPos", m_Scene->GetMainCamera()->GetTransform().GetPosition());
        shader.SetUniformVec2("u_ScreenSize", glm::vec2(m_GBuffer->GetBufferWidth(), m_GBuffer->GetBufferHeight()));

        const std::string u_name = "u_DirectionalLight.";
        shader.SetUniformVec3(u_name + "color", light->color);
        shader.SetUniform1f(u_name + "intensity", light->intensity);
        shader.SetUniformVec3(u_name + "direction", light->direction);
    }

    void RenderHandler::DrawDirLight(const std::shared_ptr<DirectionalLight>& light)
    {
        const auto& quad = rgr::Mesh::GetBuiltInMesh(rgr::Mesh::BUILT_IN_MESHES::QUAD_NDC_FULLSCREEN);

        m_DirLightsDepthAtlas->BindToSlot(3);

        if (!light->castShadows)
        {
            const auto& shader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::DIR_LIGHT_NO_SHADOWS);
            shader.Bind();
            SetDirLightCommonUniforms(light, shader);

            quad.DrawElements();
            return;
        }

        const auto& shader = light->castShadows && !light->smoothShadows ?
                 rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::DIR_LIGHT_SHADOWS_NO_PCF) :
                 rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::DIR_LIGHT_SHADOWS_PCF);

        shader.Bind();

        const std::string u_name = "u_DirectionalLight.";
        shader.SetUniformMat4(u_name + "lightSpaceViewProj", false, light->GetLightSpaceViewProj());
        shader.SetUniformVec2(u_name + "atlasOffset", light->atlasOffset);
        SetDirLightCommonUniforms(light, shader);

        quad.DrawElements();
    }

    void RenderHandler::DoLightingPass()
    {
        auto camera = m_Scene->GetMainCamera();
        const auto& lights = m_Scene->GetLightsAround(camera->GetTransform().GetPosition(),camera->shadowsVisibilityDistance);

        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

        m_GBuffer->BindPositionTexture();
        m_GBuffer->BindNormalTexture();
        m_GBuffer->BindColorTexture();

        for (const auto& light : lights)
        {
            if (auto dirLight = std::dynamic_pointer_cast<DirectionalLight>(light))
            {
                DrawDirLight(dirLight);
            }
            else if (auto pointLight = std::dynamic_pointer_cast<PointLight>(light))
            {
                const auto& mesh = rgr::Mesh::GetBuiltInMesh(rgr::Mesh::BUILT_IN_MESHES::SPHERE);
                const auto& shader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::PLAIN_COLOR);

                const auto mvp = camera->GetPerspective() * camera->GetView() * pointLight->GetLightVolumeModelMatrix();

                shader.Bind();
                shader.SetUniformVec4("u_Color", glm::vec4(1, 1, 1, 1));
                shader.SetUniformMat4("u_MVP", true, mvp);
                mesh.DrawElements();
            }
            else if (auto spotLight = std::dynamic_pointer_cast<SpotLight>(light))
            {
                const auto& mesh = rgr::Mesh::GetBuiltInMesh(rgr::Mesh::BUILT_IN_MESHES::CONE);
                const auto& shader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::PLAIN_COLOR);

                const auto mvp = camera->GetPerspective() * camera->GetView() * spotLight->GetLightVolumeModelMatrix();

                shader.Bind();
                shader.SetUniformVec4("u_Color", glm::vec4(1, 1, 1, 1));
                shader.SetUniformMat4("u_MVP", true, mvp);
                mesh.DrawElements();
            }
        }

        glDisable(GL_BLEND);
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
        glBlitFramebuffer(0, 0, static_cast<int>(m_GBuffer->GetBufferWidth()), static_cast<int>(m_GBuffer->GetBufferHeight()),
                          0, 0, static_cast<int>(m_GBuffer->GetBufferWidth()), static_cast<int>(m_GBuffer->GetBufferHeight()),
                          GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void RenderHandler::InitializeDepthAtlases()
    {
        constexpr float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

        // Directional lights atlas
        m_DirLightsDepthAtlas = std::make_unique<rgr::Texture>(DIR_LIGHT_ATLAS_SIZE, DIR_LIGHT_ATLAS_SIZE, rgr::Texture::TYPE::DEPTH_COMPONENT);
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

    void RenderHandler::SetShadersConstantUniforms()
    {
        // Shader for directional lights with shadows and without pcf
        const auto& dirLightNoPCF = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::DIR_LIGHT_SHADOWS_NO_PCF);
        dirLightNoPCF.Bind();
        dirLightNoPCF.SetUniform1is("g_Position", 0);
        dirLightNoPCF.SetUniform1is("g_Normal", 1);
        dirLightNoPCF.SetUniform1is("g_AlbedoSpec", 2);
        dirLightNoPCF.SetUniform1is("u_DepthMapAtlas", 3);
        dirLightNoPCF.SetUniform1f("u_OneShadowMapOffset", (1.0f / DIR_LIGHT_MAPS_PER_ATLAS_AXIS));

        // Shader for directional lights with shadows and without pcf
        const auto& dirLightPCF = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::DIR_LIGHT_SHADOWS_PCF);
        dirLightPCF.Bind();
        dirLightPCF.SetUniform1is("g_Position", 0);
        dirLightPCF.SetUniform1is("g_Normal", 1);
        dirLightPCF.SetUniform1is("g_AlbedoSpec", 2);
        dirLightPCF.SetUniform1is("u_DepthMapAtlas", 3);
        dirLightPCF.SetUniform1f("u_OneShadowMapOffset", (1.0f / DIR_LIGHT_MAPS_PER_ATLAS_AXIS));

        // Shader for directional lights without shadows
        const auto& dirLightNoShadows = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::DIR_LIGHT_NO_SHADOWS);
        dirLightNoShadows.Bind();
        dirLightNoShadows.SetUniform1is("g_Position", 0);
        dirLightNoShadows.SetUniform1is("g_Normal", 1);
        dirLightNoShadows.SetUniform1is("g_AlbedoSpec", 2);
    }
}

//glViewport(0, 0, 900, 900);
//rgr::Mesh* quad = rgr::Mesh::Get2DQuadMesh();
//rgr::Shader* shader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::TEXTURE_TEST);
//
//shader->BindToSlot();
//glActiveTexture(GL_TEXTURE0);
//glBindTexture(GL_TEXTURE_2D, m_SpotLightsDepthAtlasHandle);
//shader->SetUniform1i("u_Texture", m_SpotLightsDepthAtlasHandle);
//
//quad->Draw();
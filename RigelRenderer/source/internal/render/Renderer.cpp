#include "Renderer.hpp"

#include "GBuffer.hpp"
#include "RigelRenderer.hpp"
#include "mesh/Mesh.hpp"
#include "glAbstraction/Renderbuffer.hpp"

#include "glad.h"

#include <vector>
#include <iostream>

namespace rgr
{

    Renderer::Renderer()
        : m_Scene(nullptr)
    {
        m_GBuffer = std::make_unique<rgr::GBuffer>(rgr::Core::GetWindowSize().x, rgr::Core::GetWindowSize().y);

        InitializeShadowAtlases();
        SetShadersConstantUniforms();
        InitializeLightingPass();
    }

    Renderer::~Renderer()
    {
        // Delete shadow mapping framebuffers
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &m_DirLightsFBOHandle);
        glDeleteFramebuffers(1, &m_SpotLightsFBOHandle);
    }

    void Renderer::GenerateDepthMaps()
    {
        auto camera = m_Scene->GetMainCamera();
        const auto& lights = m_Scene->GetLightsAround(camera->GetTransform().GetPosition(), camera->shadowsVisibilityDistance);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        ClearDepthAtlases();

        size_t dirLightsCount = 0;
        size_t spotLightsCount = 0;

        for (const auto& light : lights)
        {
            if (!light->castShadows) continue;

            if (auto dirLight = std::dynamic_pointer_cast<rgr::DirectionalLight>(light))
            {
                const auto x_pos = static_cast<size_t>(dirLightsCount % DIR_LIGHT_MAPS_PER_ATLAS_AXIS) * DIR_LIGHT_SHADOW_MAP_SIZE;
                const auto y_pos = static_cast<size_t>(dirLightsCount / DIR_LIGHT_MAPS_PER_ATLAS_AXIS) * DIR_LIGHT_SHADOW_MAP_SIZE;

                glBindFramebuffer(GL_FRAMEBUFFER, m_DirLightsFBOHandle);
                glViewport(static_cast<int>(x_pos), static_cast<int>(y_pos), DIR_LIGHT_SHADOW_MAP_SIZE, DIR_LIGHT_SHADOW_MAP_SIZE);

                dirLight->GenerateDepthMap();
                dirLight->atlasOffset = glm::vec2(x_pos / DIR_LIGHT_ATLAS_SIZE, y_pos / DIR_LIGHT_ATLAS_SIZE);

                dirLightsCount++;
            }
            else if (auto spotLight = std::dynamic_pointer_cast<rgr::SpotLight>(light))
            {
                const auto x_pos = static_cast<size_t>(spotLightsCount % SPOT_LIGHT_MAPS_PER_ATLAS_AXIS) * SPOT_LIGHT_SHADOW_MAP_SIZE;
                const auto y_pos = static_cast<size_t>(spotLightsCount / SPOT_LIGHT_MAPS_PER_ATLAS_AXIS) * SPOT_LIGHT_SHADOW_MAP_SIZE;

                glBindFramebuffer(GL_FRAMEBUFFER, m_SpotLightsFBOHandle);
                glViewport(static_cast<int>(x_pos), static_cast<int>(y_pos), SPOT_LIGHT_SHADOW_MAP_SIZE, SPOT_LIGHT_SHADOW_MAP_SIZE);

                spotLight->GenerateDepthMap();
                spotLight->atlasOffset = glm::vec2(x_pos / SPOT_LIGHT_ATLAS_SIZE, y_pos / SPOT_LIGHT_ATLAS_SIZE);

                spotLightsCount++;
            }
        }

        const glm::vec2 size = rgr::Core::GetWindowSize();
        glViewport(0, 0, static_cast<int>(size.x), static_cast<int>(size.y));
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glCullFace(GL_BACK);
        glDisable(GL_DEPTH_TEST);
    }

    void Renderer::DoGeometryPass()
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
        glDisable(GL_CULL_FACE);
        glDepthMask(GL_FALSE);
    }

    void Renderer::DrawDirLight(const std::shared_ptr<DirectionalLight>& light)
    {
        auto setCommonUniforms = [this](const std::shared_ptr<DirectionalLight>& light, const Shader& shader)
        {
            shader.SetUniformVec3("u_ViewPos", m_Scene->GetMainCamera()->GetTransform().GetPosition());
            shader.SetUniformVec2("u_ScreenSize", glm::vec2(m_GBuffer->GetBufferWidth(), m_GBuffer->GetBufferHeight()));

            const std::string u_name = "u_DirectionalLight.";
            shader.SetUniformVec3(u_name + "color", light->color);
            shader.SetUniform1f(u_name + "intensity", light->intensity);
            shader.SetUniformVec3(u_name + "direction", light->direction);
        };

        auto setUniformsForShadows = [](const std::shared_ptr<DirectionalLight>& light, const Shader& shader)
        {
            const std::string u_name = "u_DirectionalLight.";
            shader.SetUniformMat4(u_name + "lightSpaceViewProj", false, light->GetLightSpaceViewProj());
            shader.SetUniformVec2(u_name + "atlasOffset", light->atlasOffset);
        };

        const auto& quad = rgr::Mesh::GetBuiltInMesh(rgr::Mesh::BUILT_IN_MESHES::QUAD_NDC_FULLSCREEN);

        m_DirLightsDepthAtlas->BindToSlot(3);

        if (!light->castShadows) // light without shadows
        {
            const auto& shader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::DIR_LIGHT_NO_SHADOWS);
            shader.Bind();
            setCommonUniforms(light, shader);
        }
        else if (light->castShadows && !light->smoothShadows) // light with hard shadows (no PCF)
        {
            const auto& shader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::DIR_LIGHT_SHADOWS_NO_PCF);
            shader.Bind();
            setUniformsForShadows(light, shader);
            setCommonUniforms(light, shader);
        }
        else if (light->castShadows && light->smoothShadows) // light with smooth shadows (PCF)
        {
            const auto& shader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::DIR_LIGHT_SHADOWS_PCF);
            shader.Bind();
            setUniformsForShadows(light, shader);
            setCommonUniforms(light, shader);
        }

        quad.DrawElements();
    }

    void Renderer::DrawPointLight(const std::shared_ptr<PointLight>& light)
    {
        auto camera = m_Scene->GetMainCamera();
        const auto& mesh = rgr::Mesh::GetBuiltInMesh(rgr::Mesh::BUILT_IN_MESHES::SPHERE);
        const auto& shader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::POINT_LIGHT_NO_SHADOWS);

        const auto mvp = camera->GetPerspective() * camera->GetView() * light->GetLightVolumeModelMatrix();

        shader.Bind();

        shader.SetUniformMat4("u_MVP", true, mvp);

        const std::string u_name = "u_PointLight.";
        shader.SetUniformVec3(u_name + "color", light->color);
        shader.SetUniform1f(u_name + "intensity", light->intensity);
        shader.SetUniformVec3(u_name + "position", light->GetTransform().GetPosition());
        shader.SetUniform1f(u_name + "constant", light->constant);
        shader.SetUniform1f(u_name + "linear", light->linear);
        shader.SetUniform1f(u_name + "quadratic", light->quadratic);

        shader.SetUniformVec3("u_ViewPos", m_Scene->GetMainCamera()->GetTransform().GetPosition());
        shader.SetUniformVec2("u_ScreenSize", glm::vec2(m_GBuffer->GetBufferWidth(), m_GBuffer->GetBufferHeight()));

        mesh.DrawElements();
    }

    void Renderer::DrawSpotLight(const std::shared_ptr<SpotLight>& light)
    {
        auto setCommonUniforms = [this](const std::shared_ptr<SpotLight>& light, const Shader& shader)
        {
            auto camera = m_Scene->GetMainCamera();

            const auto mvp = camera->GetPerspective() * camera->GetView() * light->GetLightVolumeModelMatrix();
            shader.SetUniformMat4("u_MVP", true, mvp);

            const std::string u_name = "u_SpotLight.";
            shader.SetUniformVec3(u_name + "color", light->color);
            shader.SetUniform1f(u_name + "intensity", light->intensity);
            shader.SetUniformVec3(u_name + "position", light->GetTransform().GetPosition());
            shader.SetUniformVec3(u_name + "direction", light->direction);
            shader.SetUniform1f(u_name + "cutOff", light->GetCutOff());
            shader.SetUniform1f(u_name + "outerCutOff", light->outerCutOff);
            shader.SetUniform1f(u_name + "constant", light->constant);
            shader.SetUniform1f(u_name + "linear", light->linear);
            shader.SetUniform1f(u_name + "quadratic", light->quadratic);

            shader.SetUniformVec3("u_ViewPos", camera->GetTransform().GetPosition());
            shader.SetUniformVec2("u_ScreenSize", glm::vec2(m_GBuffer->GetBufferWidth(), m_GBuffer->GetBufferHeight()));
        };

        const auto& mesh = rgr::Mesh::GetBuiltInMesh(rgr::Mesh::BUILT_IN_MESHES::CONE);

        if (!light->castShadows) // light without shadows
        {
            const auto& shader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::SPOT_LIGHT_NO_SHADOWS);
            shader.Bind();
            setCommonUniforms(light, shader);
        }
        else if (light->castShadows && !light->smoothShadows) // light with hard shadows (no PCF)
        {

        }
        else if (light->castShadows && light->smoothShadows) // light with smooth shadows (PCF)
        {

        }

        mesh.DrawElements();
    }

    void Renderer::DoStencilPass()
    {
        glEnable(GL_DEPTH_TEST);

        glStencilFunc(GL_ALWAYS, 0, 0);

        glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
        glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

        auto camera = m_Scene->GetMainCamera();
        const auto& lights = m_Scene->GetLightsAround(camera->GetTransform().GetPosition(),camera->shadowsVisibilityDistance);

        for (const auto& light : lights)
        {
            if (auto pointLight = std::dynamic_pointer_cast<PointLight>(light))
            {
                const auto& mesh = rgr::Mesh::GetBuiltInMesh(rgr::Mesh::BUILT_IN_MESHES::SPHERE);
                const auto& shader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::STENCIL_PASS);
                const auto mvp = camera->GetPerspective() * camera->GetView() * pointLight->GetLightVolumeModelMatrix();

                shader.Bind();
                shader.SetUniformMat4("u_MVP", true, mvp);
                mesh.DrawElements();
            }
            else if (auto spotLight = std::dynamic_pointer_cast<SpotLight>(light))
            {
                const auto& mesh = rgr::Mesh::GetBuiltInMesh(rgr::Mesh::BUILT_IN_MESHES::CONE);
                const auto& shader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::STENCIL_PASS);
                const auto mvp = camera->GetPerspective() * camera->GetView() * spotLight->GetLightVolumeModelMatrix();

                shader.Bind();
                shader.SetUniformMat4("u_MVP", true, mvp);
                mesh.DrawElements();
            }
        }

        glDisable(GL_DEPTH_TEST);
    }

    void Renderer::DoFinalPass()
    {
        glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        auto camera = m_Scene->GetMainCamera();
        const auto& lights = m_Scene->GetLightsAround(camera->GetTransform().GetPosition(),camera->shadowsVisibilityDistance);

        for (const auto& light : lights)
        {
            if (auto pointLight = std::dynamic_pointer_cast<PointLight>(light))
                DrawPointLight(pointLight);
            else if (auto spotLight = std::dynamic_pointer_cast<SpotLight>(light))
                DrawSpotLight(spotLight);
        }

        glDisable(GL_CULL_FACE);
    }

    void Renderer::DoLightingPass()
    {
        auto camera = m_Scene->GetMainCamera();
        const auto& lights = m_Scene->GetLightsAround(camera->GetTransform().GetPosition(),camera->shadowsVisibilityDistance);

        BlitForStencilPass();

        glBindFramebuffer(GL_FRAMEBUFFER, m_LightingPassFBOHandle);
        glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

        m_GBuffer->BindPositionTexture();
        m_GBuffer->BindNormalTexture();
        m_GBuffer->BindColorTexture();

        // Process directional lighting before any fragments have been marked by stencil buffer
        for (const auto& light : lights)
            if (auto dirLight = std::dynamic_pointer_cast<DirectionalLight>(light))
                DrawDirLight(dirLight);

        glEnable(GL_STENCIL_TEST);
        DoStencilPass();
        DoFinalPass();
        glDisable(GL_STENCIL_TEST);

        glDisable(GL_BLEND);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        DrawDeferredRenderingResult();
    }

    void Renderer::DoForwardPass()
    {

    }

    void Renderer::InitializeShadowAtlases()
    {
        constexpr float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

        // Directional lights atlas
        m_DirLightsDepthAtlas = std::make_unique<rgr::Texture>(DIR_LIGHT_ATLAS_SIZE, DIR_LIGHT_ATLAS_SIZE, rgr::Texture::TYPE::DEPTH_COMPONENT);
        m_DirLightsDepthAtlas->SetFilter(rgr::Texture::FILTER::NEAREST, rgr::Texture::FILTER::NEAREST);
        m_DirLightsDepthAtlas->SetWrap(rgr::Texture::WRAP::CLAMP_TO_BORDER, rgr::Texture::WRAP::CLAMP_TO_BORDER);
        m_DirLightsDepthAtlas->SetBorderColor(borderColor);

        glGenFramebuffers(1, &m_DirLightsFBOHandle);
        glBindFramebuffer(GL_FRAMEBUFFER, m_DirLightsFBOHandle);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DirLightsDepthAtlas->GetHandle(), 0);

        // Spotlights atlas
        m_SpotLightsDepthAtlas = std::make_unique<rgr::Texture>(SPOT_LIGHT_ATLAS_SIZE, SPOT_LIGHT_ATLAS_SIZE, rgr::Texture::TYPE::DEPTH_COMPONENT);

        glGenFramebuffers(1, &m_SpotLightsFBOHandle);
        glBindFramebuffer(GL_FRAMEBUFFER, m_SpotLightsFBOHandle);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_SpotLightsDepthAtlas->GetHandle(), 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Renderer::InitializeLightingPass()
    {
        const glm::vec2 size = rgr::Core::GetWindowSize();

        glGenFramebuffers(1, &m_LightingPassFBOHandle);
        glBindFramebuffer(GL_FRAMEBUFFER, m_LightingPassFBOHandle);

        m_LightingPassColorTexture = std::make_unique<rgr::Texture>(size.x, size.y, rgr::Texture::TYPE::RGBA);
        m_LightingPassColorTexture->SetFilter(rgr::Texture::FILTER::NEAREST, rgr::Texture::FILTER::NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_LightingPassColorTexture->GetHandle(), 0);

        m_LightingPassStencilRenderbuffer = std::make_unique<rgr::Renderbuffer>(size.x, size.y, rgr::Renderbuffer::Type::DEPTH24_STENCIL8);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_LightingPassStencilRenderbuffer->GetHandle());

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Renderer::ClearDepthAtlases() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_DirLightsFBOHandle);
        glClear(GL_DEPTH_BUFFER_BIT);
        glBindFramebuffer(GL_FRAMEBUFFER, m_SpotLightsFBOHandle);
        glClear(GL_DEPTH_BUFFER_BIT);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Renderer::SetShadersConstantUniforms()
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

        // Shader for point lights without shadows
        const auto& pointLightNoShadows = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::POINT_LIGHT_NO_SHADOWS);
        pointLightNoShadows.Bind();
        pointLightNoShadows.SetUniform1is("g_Position", 0);
        pointLightNoShadows.SetUniform1is("g_Normal", 1);
        pointLightNoShadows.SetUniform1is("g_AlbedoSpec", 2);

        // Shader for spot light without shadows
        const auto& spotLightNoShadows = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::SPOT_LIGHT_NO_SHADOWS);
        spotLightNoShadows.Bind();
        spotLightNoShadows.SetUniform1is("g_Position", 0);
        spotLightNoShadows.SetUniform1is("g_Normal", 1);
        spotLightNoShadows.SetUniform1is("g_AlbedoSpec", 2);

        spotLightNoShadows.Unbind();
    }

    void Renderer::RenderScene(rgr::Scene* scene)
    {
        m_Scene = scene;

        // Methods must stay in this order
        GenerateDepthMaps();
        DoGeometryPass();
        DoLightingPass();
        DoForwardPass();
    }

    void Renderer::DrawDeferredRenderingResult() const
    {
        const auto& mesh = rgr::Mesh::GetBuiltInMesh(rgr::Mesh::BUILT_IN_MESHES::QUAD_NDC_FULLSCREEN);
        const auto& shader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::FULLSCREEN_TEXTURE);

        shader.Bind();

        shader.BindTexture("u_Texture", m_LightingPassColorTexture.get(), 0);

        mesh.DrawElements();
    }

    void Renderer::BlitForStencilPass() const
    {
        /*
         * Copy the depth values from the GBuffer to the lighting pass framebuffer to be able
         * to perform depth testing of light volume's fragments against the depth values stored in the GBuffer
         */

        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GBuffer->GetFBOHandle());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_LightingPassFBOHandle);

        const glm::vec2 size = rgr::Core::GetWindowSize();
        const auto width = static_cast<int>(size.x);
        const auto height = static_cast<int>(size.y);

        glBlitFramebuffer(0, 0, width, height,
                          0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }
}

#include "RenderHandler.hpp"

#include "GBuffer.hpp"
#include "RigelRenderer.hpp"

#include "glew.h"

#include <vector>

namespace rgr
{

    RenderHandler::RenderHandler(rgr::Scene* scene)
    {
        m_Scene = scene;
        m_GBuffer = std::make_unique<rgr::GBuffer>(rgr::GetViewportSize().width, rgr::GetViewportSize().height);

        InitializeDepthMapFBO();
    }

    RenderHandler::~RenderHandler()
    {
        DeleteDepthMapFBO();
    }

    void RenderHandler::GenerateDepthMaps()
    {
        rgr::Camera* camera = m_Scene->GetMainCamera();
        const auto& lights = m_Scene->GetLightsAround(camera->GetTransform().GetPosition(), camera->shadowsVisibilityDistance);

        for (auto light : lights)
        {
            if (!light->castShadows) continue;
            light->GenerateDepthMap(m_DepthMapFBOHandle);
        }
    }

    void RenderHandler::DoGeometryPass()
    {
        const auto& renderables = m_Scene->GetRenderablesInFrustum();
        rgr::Shader* shader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::GEOMETRY_PASS);
        const glm::mat4 viewProj = m_Scene->GetMainCamera()->GetPerspective() * m_Scene->GetMainCamera()->GetView();

        m_GBuffer->Bind();
        m_GBuffer->Clear();

        shader->Bind();

        for (auto i : renderables)
        {
            auto renderable = dynamic_cast<rgr::RenderableMesh*>(i);
            if (renderable == nullptr) continue;
            renderable->RenderGeometry(shader, viewProj);
        }

        m_GBuffer->Unbind();
    }

    static void SetDirLightUniforms(const rgr::DirectionalLight* light, rgr::Shader* shader, const size_t lightIndex)
    {
        std::string u_name = "u_DirectionalLights[" + std::to_string(lightIndex) + "].";
        shader->SetUniformVec3(u_name + "color", light->color);
        shader->SetUniform1f(u_name + "intensity", light->intensity);
        shader->SetUniformVec3(u_name + "direction", light->direction);
    }

    static void SetPointLightUniforms(const rgr::PointLight* light, rgr::Shader* shader, const size_t lightIndex)
    {

    }

    static void SetSpotLightUniforms(const rgr::SpotLight* light, rgr::Shader* shader, const size_t lightIndex)
    {

    }

    void RenderHandler::DoLightingPass()
    {
        rgr::Camera* camera = m_Scene->GetMainCamera();
        const auto& lights = m_Scene->GetLightsAround(camera->GetTransform().GetPosition(),camera->shadowsVisibilityDistance);
        rgr::Shader* shader = rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::LIGHTING_PASS);
        rgr::Mesh* quad = rgr::Mesh::Get2DQuadMesh();

        m_GBuffer->Clear();
        shader->Bind();

        shader->SetUniform1i("g_Position", 0);
        shader->SetUniform1i("g_Normal", 1);
        shader->SetUniform1i("g_AlbedoSpec", 2);
        shader->SetUniformVec3("u_ViewPos", camera->GetTransform().GetPosition());

        m_GBuffer->BindPositionTexture();
        m_GBuffer->BindNormalTexture();
        m_GBuffer->BindColorTexture();

        size_t dirCount = 0;

        for (auto light: lights)
        {
            if (auto dirLight = dynamic_cast<DirectionalLight*>(light))
            {
                if (dirCount > (MAX_DIR_LIGHTS_COUNT - 1)) continue;
                SetDirLightUniforms(dirLight, shader, dirCount);
                dirCount++;
            }
        }

        shader->SetUniform1i("u_DirLightsCount", dirCount);

        quad->Draw();

        BlitDeferredFBO();
    }

    void RenderHandler::DoForwardPass()
    {

    }

    void RenderHandler::InitializeDepthMapFBO()
    {
        // Note that attaching a depth map texture is skipped, because one will later be attached for each light individually
        glGenFramebuffers(1, &m_DepthMapFBOHandle);
        glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBOHandle);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void RenderHandler::DeleteDepthMapFBO()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &m_DepthMapFBOHandle);
    }

    void RenderHandler::BlitDeferredFBO()
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GBuffer->GetFBOHandle());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, m_GBuffer->GetBufferWidth(), m_GBuffer->GetBufferHeight(),
                          0, 0, m_GBuffer->GetBufferWidth(), m_GBuffer->GetBufferHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

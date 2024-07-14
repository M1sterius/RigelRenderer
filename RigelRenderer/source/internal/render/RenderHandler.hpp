#pragma once

#include <memory>

namespace rgr
{
    class Scene;
    class GBuffer;
    class DirectionalLight;
    class SpotLight;
    class PointLight;
    class Shader;
    class Texture;

    class RenderHandler
    {
    public:
        explicit RenderHandler(rgr::Scene* scene);
        ~RenderHandler();

        void GenerateDepthMaps();
        void DoGeometryPass();
        void DoLightingPass();
        void DoForwardPass();
    private:
        static constexpr size_t MAX_DIR_LIGHTS_COUNT = 9;
        static constexpr size_t MAX_SPOT_LIGHTS_COUNT = 64;
        static constexpr size_t MAX_POINT_LIGHTS_COUNT = 64;

        rgr::Scene* m_Scene;
        std::unique_ptr<GBuffer> m_GBuffer;
        std::unique_ptr<Texture> m_DirLightsDepthAtlas;
        std::unique_ptr<Texture> m_SpotLightsDepthAtlas;

        unsigned int m_DirLightsFBOHandle = 0;
        unsigned int m_SpotLightsFBOHandle = 0;

        void InitializeDepthMapFBOs();
        void DeleteDepthMapFBOs();
        void BlitDeferredFBO();
        void ClearDepthAtlases() const;
        void InitializeDepthAtlases();

        static void SetDirLightUniforms(rgr::DirectionalLight* light, rgr::Shader& shader, const size_t lightIndex);
        static void SetSpotLightUniforms(rgr::SpotLight* light, rgr::Shader& shader, const size_t lightIndex);
        static void SetPointLightUniforms(rgr::PointLight* light, rgr::Shader& shader, const size_t lightIndex);
    };
}

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
        RenderHandler();
        ~RenderHandler();

        void SetScene(rgr::Scene* scene);

        void GenerateDepthMaps();
        void DoGeometryPass();
        void DoLightingPass();
        void DoForwardPass();
    private:
        static constexpr size_t DIR_LIGHT_SHADOW_MAP_SIZE = 2048;
        static constexpr size_t DIR_LIGHT_MAPS_PER_ATLAS_AXIS = 3;
        static constexpr size_t DIR_LIGHT_ATLAS_SIZE = DIR_LIGHT_SHADOW_MAP_SIZE * DIR_LIGHT_MAPS_PER_ATLAS_AXIS;

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
        static void SetShadersConstantUniforms();

        static void SetDirLightUniforms(const std::shared_ptr<DirectionalLight>& light, const rgr::Shader& shader, const size_t lightIndex);
        static void SetSpotLightUniforms(const std::shared_ptr<SpotLight>& light, const rgr::Shader& shader, const size_t lightIndex);
        static void SetPointLightUniforms(const std::shared_ptr<PointLight>& light, const rgr::Shader& shader, const size_t lightIndex);

        void DrawDirLight(const std::shared_ptr<DirectionalLight>& light);
//        static void DrawSpotLight(const std::shared_ptr<PointLight>& light);
//        static void DrawPointLight(const std::shared_ptr<PointLight>& light);
    };
}

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
        static constexpr size_t DIR_LIGHT_MAX_SHADOW_MAPS_IN_ATLAS = DIR_LIGHT_MAPS_PER_ATLAS_AXIS * DIR_LIGHT_MAPS_PER_ATLAS_AXIS;

        rgr::Scene* m_Scene;
        std::unique_ptr<GBuffer> m_GBuffer;
        std::unique_ptr<Texture> m_DirLightsDepthAtlas;
        std::unique_ptr<Texture> m_SpotLightsDepthAtlas;

        unsigned int m_DirLightsFBOHandle = 0;
        unsigned int m_SpotLightsFBOHandle = 0;

        void InitializeDepthMapFBOs();
        void DeleteDepthMapFBOs();
        void ClearDepthAtlases() const;
        void InitializeDepthAtlases();
        static void SetShadersConstantUniforms();

        void DrawDirLight(const std::shared_ptr<DirectionalLight>& light);
        void SetDirLightCommonUniforms(const std::shared_ptr<DirectionalLight>& light, const rgr::Shader& shader);
//        static void DrawSpotLight(const std::shared_ptr<PointLight>& light);
//        static void DrawPointLight(const std::shared_ptr<PointLight>& light);
    };
}

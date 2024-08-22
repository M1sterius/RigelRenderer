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
    class Renderbuffer;

    class Renderer
    {
    public:
        Renderer();
        ~Renderer();

        void RenderScene(rgr::Scene* scene);
    private:
        static constexpr size_t DIR_LIGHT_SHADOW_MAP_SIZE = 2048;
        static constexpr size_t DIR_LIGHT_MAPS_PER_ATLAS_AXIS = 3;
        static constexpr size_t DIR_LIGHT_ATLAS_SIZE = DIR_LIGHT_SHADOW_MAP_SIZE * DIR_LIGHT_MAPS_PER_ATLAS_AXIS;
        static constexpr size_t DIR_LIGHT_MAX_SHADOW_MAPS_IN_ATLAS = DIR_LIGHT_MAPS_PER_ATLAS_AXIS * DIR_LIGHT_MAPS_PER_ATLAS_AXIS;

        rgr::Scene* m_Scene;
        std::unique_ptr<GBuffer> m_GBuffer;
        std::unique_ptr<Texture> m_DirLightsDepthAtlas;
        std::unique_ptr<Texture> m_SpotLightsDepthAtlas;

        std::unique_ptr<Texture> m_LightingPassColorTexture;
        std::unique_ptr<Renderbuffer> m_LightingPassStencilRenderbuffer;

        unsigned int m_DirLightsFBOHandle = 0;
        unsigned int m_SpotLightsFBOHandle = 0;
        unsigned int m_LightingPassFBOHandle = 0;

        void GenerateDepthMaps();
        void DoGeometryPass();
        void DoLightingPass();
        void DoForwardPass();

        void DoStencilPass();
        void DoFinalPass();

        void ClearDepthAtlases() const;
        void InitializeShadowAtlases();
        void InitializeLightingPass();
        void DrawFinalDeferredScreen() const;
        void BlitForStencilPass() const;

        static void SetShadersConstantUniforms();

        void DrawDirLight(const std::shared_ptr<DirectionalLight>& light);
        void SetDirLightCommonUniforms(const std::shared_ptr<DirectionalLight>& light, const rgr::Shader& shader);
//        static void DrawSpotLight(const std::shared_ptr<PointLight>& light);
//        static void DrawPointLight(const std::shared_ptr<PointLight>& light);
    };
}

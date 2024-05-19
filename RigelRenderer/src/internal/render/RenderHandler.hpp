#pragma once

#include <memory>

namespace rgr
{
    class Scene;
    class GBuffer;

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
        static const size_t MAX_DIR_LIGHTS_COUNT = 8;
        static const size_t MAX_POINT_LIGHTS_COUNT = 64;
        static const size_t MAX_SPOT_LIGHTS_COUNT = 64;

        rgr::Scene* m_Scene;
        std::unique_ptr<GBuffer> m_GBuffer;
        unsigned int m_DepthMapFBOHandle = 0;

        void InitializeDepthMapFBO();
        void DeleteDepthMapFBO();
        void BlitDeferredFBO();
    };
}

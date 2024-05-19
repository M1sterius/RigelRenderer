#pragma once

#include "glAbstraction/GlAbstraction.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "Texture.hpp"

namespace rgr
{	
	class Scene;
	class GBuffer;
    class Shader;
    class DirectionalLight;
    class PointLight;
    class SpotLight;

	class Renderer
	{
    private:
        static unsigned int m_DepthMapFBOHandle;
        static void InitializeDepthMapFBO();

        static const size_t MAX_DIR_LIGHTS_COUNT = 8;

        static void SetDirectionalLightUniforms(const DirectionalLight* light, Shader* shader, const size_t lightIndex);
	public:
		static void GenerateDepthMapsForLightSources(const Scene* scene);

		static void DoGeometryPass(const Scene* scene, const GBuffer* gBuffer);
		static void DoLightingPass(const Scene* scene, const GBuffer* gBuffer);
		static void DoForwardPass(const Scene* scene);
	};
}

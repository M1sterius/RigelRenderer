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

	class Renderer
	{
	public:
		static void GenerateDepthMapsForLightSources(const Scene* scene);

		static void DoGeometryPass(const Scene* scene, const GBuffer* gBuffer);
		static void DoLightingPass(const Scene* scene, const GBuffer* gBuffer);
	};
}
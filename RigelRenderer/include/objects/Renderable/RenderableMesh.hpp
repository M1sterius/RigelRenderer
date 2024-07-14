#pragma once

#include "Renderable.hpp"
#include "glm.hpp"

#include <memory>

namespace rgr
{	
	class Mesh;
	class Shader;
	class Texture;

	// Represents 3D mesh affectable by light
	class RenderableMesh final : public::rgr::Renderable
	{
	private:
		std::shared_ptr<Mesh> m_Mesh;
	public:
		explicit RenderableMesh(std::shared_ptr<rgr::Mesh> mesh);
		~RenderableMesh() override;

		std::shared_ptr<Texture> diffuseTexture;
        std::shared_ptr<Texture> specularTexture;

		// Determines in what radius light will be able to affect appearance of this object
		float affectedByLightDist = 7.0f;
		bool shadowCaster = true;
	INTERNAL:
		inline std::shared_ptr<Mesh> GetMesh() { return m_Mesh; }

		void RenderDepth(const glm::mat4& lightSpaceMatrix);
		void RenderGeometry(rgr::Shader& shader, const glm::mat4& viewProj);
	};
}

#pragma once

#include "Renderable.hpp"
#include "glm.hpp"

namespace rgr
{	
	class Mesh;
	class Shader;
	class Texture;

	// Represents 3D mesh affectable by light
	class RenderableMesh final : public::rgr::Renderable
	{
	private:
		Mesh* m_Mesh;

		void SetMaterialUniforms(rgr::Shader* shader);
	public:
		RenderableMesh(rgr::Mesh* mesh);
		virtual ~RenderableMesh();

		Texture* diffuseTexture;
		Texture* specularTexture;

		// Determines in what radius light will be able to affect appearance of this object
		float affectedByLightDist = 7.0f;
		bool shadowCaster = true;
	INTERNAL:
		inline Mesh* GetMesh() { return m_Mesh; }

		void RenderDepth(const glm::mat4& lightSpaceMatrix);
		void RenderGeometry(rgr::Shader* shader, const glm::mat4& viewProj);
	};
}

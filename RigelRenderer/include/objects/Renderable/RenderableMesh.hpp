#pragma once

#include "Renderable.hpp"
#include "glm.hpp"

namespace rgr
{	
	class Mesh;
	class Material;
	class MaterialLit;
	class Shader;

	// Represents 3D mesh affectable by light
	class RenderableMesh final : public::rgr::Renderable
	{
	private:
		Mesh* m_Mesh;
		MaterialLit* m_Material;
	public:
		RenderableMesh(rgr::Mesh* mesh, rgr::MaterialLit* material);
		virtual ~RenderableMesh();

		// Determines in what radius light will be able to affect appearance of this object
		float affectedByLightDist = 7.0f;
		bool shadowCaster = true;
	INTERNAL:
		inline Mesh* GetMesh() { return m_Mesh; }
		inline MaterialLit* GetMaterial() { return m_Material; }

		void RenderDepth(const glm::mat4& lightSpaceMatrix);
		void RenderGeometry(const rgr::Shader* shader, const glm::mat4& viewProj);
	};
}

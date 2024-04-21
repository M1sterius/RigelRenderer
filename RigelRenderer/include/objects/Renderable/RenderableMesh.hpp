#pragma once

#include "Renderable.hpp"

namespace rgr
{	
	class Scene;
	class Mesh;
	class Material;

	// Represents 3D mesh affectable by light
	class RenderableMesh final : public::rgr::Renderable
	{
	private:
		Mesh* m_Mesh;
		Material* m_Material;
	public:
		RenderableMesh(rgr::Mesh* mesh, rgr::Material* material);
		virtual ~RenderableMesh();

		// Determines in what radius light will be able to affect appearance of this object
		float affectedByLightDist = 7.0f;
	INTERNAL:
		inline Mesh* GetMesh() { return m_Mesh; }
		inline Material* GetMaterial() { return m_Material; }
		void Render() override;
		void DepthRender(const glm::mat4& lightSpaceMatrix) override;
	};
}
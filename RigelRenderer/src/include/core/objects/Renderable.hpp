#pragma once

#include "core/components/Mesh.hpp"
#include "core/components/Material.hpp"
#include "core/objects/Object.hpp"

namespace rgr
{
	class Renderable : public rgr::Object
	{
	public:
		float affectedByLightDistance = 5.0f;

		Renderable(rgr::Mesh* mesh, rgr::Material* material);
		~Renderable() override;

		rgr::Mesh* GetMesh() const;
		rgr::Material* GetMaterial() const;
	private:
		rgr::Mesh* m_Mesh;
		rgr::Material* m_Material;
	};
}


#pragma once

#include "Renderable.hpp"
#include "glm.hpp"

#include <memory>

namespace rgr
{	
	class Model;
	class Shader;
	class Texture;

	// Represents 3D model affectable by light
	class RenderableModel final : public::rgr::Renderable
	{
	private:
		std::shared_ptr<rgr::Model> m_Model;
	public:
		explicit RenderableModel(std::shared_ptr<rgr::Model> mesh);
		~RenderableModel() override;

		// Determines in what radius light will be able to affect appearance of this object
		float affectedByLightDist = 7.0f;
		bool shadowCaster = true;
	INTERNAL:
		inline std::shared_ptr<Model> GetModel() { return m_Model; }

		void RenderDepth(const glm::mat4& lightSpaceMatrix);
		void RenderGeometry(rgr::Shader& shader, const glm::mat4& viewProj);
	};
}

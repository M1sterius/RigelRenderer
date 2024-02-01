#pragma once

#include "core/components/Material.hpp"
#include "core/components/Texture.hpp"
#include "vec4.hpp"

class Material2D : public rgr::Material
{
public:
	glm::vec4 color = glm::vec4(0.0f);

	Material2D(rgr::Texture* texture, rgr::Shader* shader);
	~Material2D() override;

	void SetUniforms() override;
private:
	rgr::Texture* m_Texture;
};


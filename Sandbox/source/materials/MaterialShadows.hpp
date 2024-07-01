#pragma once

#include "components/Material.hpp"
#include "components/Texture.hpp"

class MaterialShadows : public rgr::Material
{
public:
	MaterialShadows(rgr::Texture* texture, rgr::Shader* shader);
	~MaterialShadows() override;

	void SetUniforms() override;
private:
	rgr::Texture* m_Texture;
};

